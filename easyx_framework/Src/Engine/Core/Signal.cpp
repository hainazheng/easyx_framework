#include"ZHNpch.h"
#include"Signal.h"


namespace SignalSlotSystem {

    // 使用函数返回静态成员引用解决链接问题
    std::map<const void*, std::unique_ptr<ConnectionSystem::ConnectionList>>& ConnectionSystem::signalMap() {
        static std::map<const void*, std::unique_ptr<ConnectionList>> instance;
        return instance;
    }

    std::shared_mutex& ConnectionSystem::globalMutex() {
        static std::shared_mutex instance;
        return instance;
    }

    std::unordered_set<SignalObject*>& ConnectionSystem::liveObjects() {
        static std::unordered_set<SignalObject*> instance;
        return instance;
    }


    SignalObject::SignalObject() {
        ConnectionSystem::registerObject(this);
    }

     SignalObject::~SignalObject() {
            // 对象销毁时断开所有相关连接
            ConnectionSystem::unregisterObject(this);
            ConnectionSystem::disconnect(this);
            ConnectionSystem::cleanupInvalidConnections();    // 清理无效连接
        }

// 对象生命周期管理
void ConnectionSystem::registerObject(SignalObject* obj) {
    std::unique_lock<std::shared_mutex> lock(globalMutex());
    liveObjects().insert(obj);
}

void ConnectionSystem::unregisterObject(SignalObject* obj) {
    std::unique_lock<std::shared_mutex> lock(globalMutex());
    liveObjects().erase(obj);
}

// 断开与指定对象相关的所有连接
void ConnectionSystem::disconnect(SignalObject* obj) {
    std::unique_lock<std::shared_mutex> lock(globalMutex());
    for (auto& pair : signalMap()) {
        auto& connections = *pair.second;
        for (auto& conn : connections) {
            if (conn.receiver == obj) {
                conn.valid = false;
            }
        }
    }
}

// 清理无效连接
void ConnectionSystem::cleanupInvalidConnections() {
    std::unique_lock<std::shared_mutex> lock(globalMutex());
    for (auto& pair : signalMap()) {
        if (pair.second) {
            auto& connections = *pair.second;
            // 移除无效连接
            connections.erase(
                std::remove_if(connections.begin(), connections.end(),
                    [](const Connection& conn) {
                        return !conn.valid;
                    }
                ),
                connections.end()
            );
        }
    }
}

}