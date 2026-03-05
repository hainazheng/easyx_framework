#pragma once
#include"ZHNpch.h"

// 命名空间定义
namespace SignalSlotSystem {

    // 前置声明
    class SignalObject;

    /**
     * @brief 信号槽连接系统核心类
     */
    class ConnectionSystem {
    private:
        struct Connection {
            SignalObject* receiver = nullptr;
            std::function<void()> slot;
            std::atomic<bool> valid{ true };
            uint64_t lambdaId = 0;             // Lambda 连接的唯一标识


            // 显式定义移动构造函数
            Connection(Connection&& other) noexcept
                : receiver(other.receiver),
                slot(std::move(other.slot)),
                valid(other.valid.load()),
                lambdaId(other.lambdaId) {}

            // 显式定义移动赋值运算符
            Connection& operator=(Connection&& other) noexcept {
                if (this != &other) {
                    receiver = other.receiver;
                    slot = std::move(other.slot);
                    valid = other.valid.load();
                    lambdaId = other.lambdaId;
                }
                return *this;
            }

            // 禁用赋值运算符
            Connection& operator=(const Connection&) = delete;
            // 禁用拷贝构造函数
            Connection(const Connection&) = delete;

            Connection() {};
        };

        using ConnectionList = std::list<Connection>;
        // 使用单例模式解决静态成员问题
        static std::map<const void*, std::unique_ptr<ConnectionList>>& signalMap();
        static std::shared_mutex& globalMutex();  // 使用共享互斥锁& globalMutex();
        static std::unordered_set<SignalObject*>& liveObjects();
        std::atomic<uint64_t> nextLambdaId{ 1 }; // Lambda ID 生成器


    public:
        template <typename Sender, typename Signal, typename Receiver, typename Slot>
        static void connect(Sender* sender, Signal signal, Receiver* receiver, Slot slot);

        template <typename Sender, typename Signal>
        static void disconnect(Sender* sender, Signal signal, SignalObject* receiver = nullptr);

        // 断开特定信号和 Lambda 的连接
        template <typename Sender, typename Signal>
        static void disconnect(Sender* sender, Signal signal, uint64_t lambdaId);

        static void disconnect(SignalObject* obj); // 断开与obj相关的所有连接

        template <typename... Args, typename Sender, typename Signal>
        static void emitSignal(Sender* sender, Signal signal, Args&&... args);

        static void registerObject(SignalObject* obj);

        static void unregisterObject(SignalObject* obj);

        // 连接函数实现（Lambda 接收者）
        template <typename Sender, typename Signal, typename Slot>
        static uint64_t connect(Sender* sender, Signal signal, Slot slot);

        // 清理无效连接
        static void cleanupInvalidConnections();
    };

    // ====================== SignalObject基类实现 ====================== //

    /**
     * @brief SignalObject基类，提供对象生命周期管理
     */
    class SignalObject {
    public:
        SignalObject();

        virtual ~SignalObject();

        // 禁用拷贝和移动
        SignalObject(const SignalObject&) = delete;
        SignalObject& operator=(const SignalObject&) = delete;
    };

    // 安全获取信号地址的辅助函数
    template <typename T>
    const void* getSignalAddress(T signal) {
        // 使用函数指针的地址作为唯一标识
        static T staticSignal = signal;
        return &staticSignal;
    }

    // 连接函数实现
    template <typename Sender, typename Signal, typename Receiver, typename Slot>
    void ConnectionSystem::connect(Sender* sender, Signal signal, Receiver* receiver, Slot slot) {
        // 获取信号函数的地址
        const void* signalAddress = getSignalAddress(signal);

        std::unique_lock<std::shared_mutex> lock(globalMutex());

        // 确保连接列表存在
        if (signalMap().find(signalAddress) == signalMap().end()) {
            signalMap()[signalAddress] = std::make_unique<ConnectionList>();
        }

        // 创建新连接（使用移动语义）
        Connection newConnection;
        newConnection.receiver = receiver;

        // 包装槽函数，检查接收者是否存活
        newConnection.slot = [receiver, slot] {
            // 先检查接收者是否存活，然后立即释放锁
            bool isAlive = false;
            {
                std::shared_lock<std::shared_mutex> lock(globalMutex());
                isAlive = (liveObjects().find(receiver) != liveObjects().end());
            }
            if (isAlive) {
                slot();
            }
            };

        // 添加到连接列表（使用移动语义）
        signalMap()[signalAddress]->emplace_back(std::move(newConnection));
    }

    // 连接函数实现（Lambda 接收者）
    template <typename Sender, typename Signal, typename Slot>
    uint64_t ConnectionSystem::connect(Sender* sender, Signal signal, Slot slot) {
        // 获取信号函数的地址
        const void* signalAddress = getSignalAddress(signal);

        // 使用独占锁
        std::unique_lock<std::shared_mutex> lock(globalMutex);

        // 确保连接列表存在
        if (signalMap().find(signalAddress) == signalMap().end()) {
            signalMap()[signalAddress] = std::make_unique<ConnectionList>();
        }

        // 创建新连接（使用移动语义）
        Connection newConnection;

        // 生成唯一 Lambda ID
        uint64_t lambdaId = nextLambdaId++;
        newConnection.lambdaId = lambdaId;

        // 包装槽函数
        newConnection.slot = [slot] {
            slot();
            };

        // 添加到连接列表（使用移动语义）
        signalMap[signalAddress]->emplace_back(std::move(newConnection));

        return lambdaId;
    }

    // 断开特定信号和接收者的连接
    template <typename Sender, typename Signal>
    void ConnectionSystem::disconnect(Sender* sender, Signal signal, SignalObject* receiver) {
        const void* signalAddress = getSignalAddress(signal);

        std::unique_lock<std::shared_mutex> lock(globalMutex());

        if (signalMap().find(signalAddress) != signalMap().end()) {
            auto& connections = *signalMap()[signalAddress];
            for (auto& conn : connections) {
                if (conn.receiver == receiver) {
                    conn.valid = false;
                }
            }
        }
        cleanupInvalidConnections();
    }

    // 断开特定信号和 Lambda 的连接
    template <typename Sender, typename Signal>
    void ConnectionSystem::disconnect(Sender* sender, Signal signal, uint64_t lambdaId) {
        const void* signalAddress = getSignalAddress(signal);

        // 使用独占锁
        std::unique_lock<std::shared_mutex> lock(globalMutex);

        if (signalMap.find(signalAddress) != signalMap.end() && signalMap[signalAddress]) {
            auto& connections = *signalMap[signalAddress];
            for (auto& conn : connections) {
                if (conn.lambdaId == lambdaId) {
                    conn.valid = false;
                }
            }
        }
    }

    // 发射信号
    template <typename... Args, typename Sender, typename Signal>
    void ConnectionSystem::emitSignal(Sender* sender, Signal signal, Args&&... args) {
        const void* signalAddress = getSignalAddress(signal);
        std::vector<std::function<void()>> slotsToCall;

        {
            std::shared_lock<std::shared_mutex> lock(globalMutex());
            if (signalMap().find(signalAddress) != signalMap().end()) {
                auto& connections = *signalMap()[signalAddress];
                for (auto& conn : connections) {
                    if (conn.valid) {
                        // 复制槽函数（std::function是可拷贝的）
                        slotsToCall.push_back(conn.slot);
                    }
                }
            }
        }

        // 执行槽函数
        for (auto& slot : slotsToCall) {
            slot();
        }
    }

    // ====================== 宏定义简化接口 ====================== //

#define SIGNAL(signal) signal
#define SLOT(receiver, slot) [&] { if(receiver!=nullptr)(receiver)->slot; }

#define CONNECT(sender, signal, receiver, slot) \
    ConnectionSystem::connect(sender, SIGNAL(signal), receiver, SLOT(receiver, slot))

#define DISCONNECT(sender, signal, receiver) \
    ConnectionSystem::disconnect(sender, SIGNAL(signal), receiver)

#define EMIT_SIGNAL(sender, signal, ...) \
    SignalSlotSystem::emitSignal(sender, SIGNAL(signal), ##__VA_ARGS__)

#define LAMBDA_SLOT(slot) [=] { slot(); }
#define CONNECT_LAMBDA(sender, signal, slot) \
    SignalSlotSystem::connect(sender, SIGNAL(signal), LAMBDA_SLOT(slot))

// 公共接口函数
    template <typename Sender, typename Signal, typename Receiver, typename Slot>
    void connect(Sender* sender, Signal signal, Receiver* receiver, Slot slot) {
        ConnectionSystem::connect(sender, signal, receiver, slot);
    }

    template <typename Sender, typename Signal>
    void disconnect(Sender* sender, Signal signal, SignalObject* receiver = nullptr) {
        ConnectionSystem::disconnect(sender, signal, receiver);
    }

    inline void disconnect(SignalObject* obj) {
        ConnectionSystem::disconnect(obj);
    }

    template <typename... Args, typename Sender, typename Signal>
    void emitSignal(Sender* sender, Signal signal, Args&&... args) {
        ConnectionSystem::emitSignal(sender, signal, std::forward<Args>(args)...);
    }

}
