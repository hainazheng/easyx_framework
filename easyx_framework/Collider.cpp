#include "Collider.h"
#include"Camera.h"
#include"easyx.h"
#include"Controller.h"
#include"GameStatics.h"


///进行函数指针映射，直接在父类统一定义使用
HitResult(*Collider::CollisionHitMap[3])(Collider*, Collider*) =
{ &Collider::CollisionHitCircleToCircle,&Collider::CollisionHitCircleToBox,&Collider::CollisionHitBoxToBox };

void CollisionDelegate::BroadCast(Collider* OverlapCollider, Object* OverlapActor)
{
    for (auto& callback : callbacks)callback(OverlapCollider,OverlapActor);
}

//圆形碰撞器和任意碰撞器判定 为正则发生了碰撞
bool CircleCollider::CollisionJudge(Collider* another)
{
    if(another->GetShape() ==ColliderShape::Circle)
    {
        CircleCollider* circle = Cast<CircleCollider>(another);
        return Vector2D::Distance(circle->GetWorldPosition(), GetWorldPosition())<=circle->GetRadius() + radius;
    }
    else
    {
        BoxCollider* box = Cast<BoxCollider>(another);
        Vector2D pos = GetWorldPosition(), pos_a = box->GetWorldPosition();
        Vector2D size = box->GetSize();
        float bottom = pos_a.y - size.y / 2.f, top = pos_a.y + size.y / 2.f,
            left = pos_a.x - size.x / 2.f, right = pos_a.x + size.x / 2.f;
        //如果圆的圆心在盒型的内部那就一定发生了碰撞
        if (pos.x <= right && pos.x >= left && pos.y <= top && pos.y >= bottom)
            return true;
        else
        {
            if (pos.x < left)
            {
                if (pos.y >= top)return Vector2D::Distance(pos, Vector2D(left, top)) <= radius;
                if (pos.y <= bottom)return Vector2D::Distance(pos, Vector2D(left, bottom)) <= radius;
                return left - pos.x <= radius;
            }
            else if (pos.x > right)
            {
                if (pos.y >= top)return Vector2D::Distance(pos, Vector2D(right, top)) <= radius;
                if (pos.y <= bottom)return Vector2D::Distance(pos, Vector2D(right, bottom)) <= radius;
                return pos.x-right <= radius;
            }
            else
            {
                if (pos.y >= top)return pos.y - top <= radius;
                else return bottom - pos.y <= radius;
            }
        }
    }
    //防止特殊情况
    return false;
}


bool CircleCollider::IsMouseOver()
{
    
    return Vector2D::Distance(GetWorldPosition(),mainWorld.mainController->GetMousePosition())<=radius;
}

void CircleCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);
    //近似一个圆
    radius = radius_ini * sqrtf(GetWorldScale().x*GetWorldScale().y);
    Vector2D pos = GetWorldPosition();
    rec_pos = { pos.x - radius, pos.y - radius, pos.x + radius, pos.y + radius };
}


void CircleCollider::DrawDebugLine()
{
    if (GetCollisionMode() == ColliderMode::None)return;
    /*setlinecolor(YELLOW);*/
   
    Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition()) 
        * (20.f / mainWorld.mainCamera->GetSpringArmLength()) + Vector2D(getwidth() / 2, getheight() / 2);
    circle((int)pos.x, (int)pos.y, int(radius * 20.f / mainWorld.mainCamera->GetSpringArmLength()));
}

//盒型碰撞器和任意碰撞器判定 为正则发生了碰撞
bool BoxCollider::CollisionJudge(Collider* another)
{
    if (another->GetShape() ==ColliderShape::Circle )
    {
        CircleCollider* circle = Cast<CircleCollider>(another);
        Vector2D pos_a = GetWorldPosition(), pos = circle->GetWorldPosition();
        Vector2D a_size = GetSize();
        float bottom = pos_a.y - a_size.y / 2.f, top = pos_a.y + a_size.y / 2.f,
            left = pos_a.x - a_size.x / 2.f, right = pos_a.x + a_size.x / 2.f;
        //如果圆的圆心在盒型的内部那就一定发生了碰撞
        if (pos.x <= right && pos.x >= left && pos.y <= top && pos.y >= bottom)
            return true;
        else
        {
            if (pos.x < left)
            {
                if (pos.y >= top)return Vector2D::Distance(pos, Vector2D(left, top)) <= circle->GetRadius();
                if (pos.y <= bottom)return Vector2D::Distance(pos, Vector2D(left, bottom)) <= circle->GetRadius();
                return left - pos.x <= circle->GetRadius();
            }
            else if (pos.x > right)
            {
                if (pos.y >= top)return Vector2D::Distance(pos, Vector2D(right, top)) <= circle->GetRadius();
                if (pos.y <= bottom)return Vector2D::Distance(pos, Vector2D(right, bottom)) <= circle->GetRadius();
                return pos.x - right <= circle->GetRadius();
            }
            else
            {
                if (pos.y >= top)return pos.y - top <= circle->GetRadius();
                else return bottom - pos.y <= circle->GetRadius();
            }
        }
    }
    else
    {
        BoxCollider* box = Cast<BoxCollider>(another);
        Vector2D pos = GetWorldPosition()-size/2.f;
        Vector2D a_pos =box-> GetWorldPosition()-box->GetSize()/2.f;
        return (pos.x<a_pos.x + box->GetSize().x && pos.x + size.x>a_pos.x
            && pos.y<a_pos.y + box->GetSize().y && pos.y + size.y>a_pos.y);
        
    }
    //防止特殊情况
    return false;
}

bool BoxCollider::IsMouseOver()
{
    Vector2D pos = GetWorldPosition(); Vector2D pos_m = mainWorld.mainController->GetMousePosition();
    return pos_m.x >= pos.x - size.x / 2 && pos_m.x <= pos.x + size.x / 2
        &&pos_m.y >= pos.y - size.y / 2 && pos_m.y <= pos.y + size.y / 2;
}

void BoxCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);
    size=size_ini*GetWorldScale();

    Vector2D pos = GetWorldPosition();
    rec_pos = { pos.x - size.x / 2,pos.y - size.y / 2,pos.x + size.x / 2,pos.y + size.y / 2 };
}

void BoxCollider::DrawDebugLine()
{
    /*std::cout << GetWorldPosition()<< mainWorld.mainCamera->GetCameraPosition() << std::endl;*/
    //物体世界中心坐标
    Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition())
        * (20.f / mainWorld.mainCamera->GetSpringArmLength()) + Vector2D(getwidth() / 2, getheight() / 2);
   /* std::cout << pos << std::endl;*/
    Vector2D si = this->size * (20.f / mainWorld.mainCamera->GetSpringArmLength());
    /*std::cout << si << std::endl;*/
    float top = pos.y - si.y / 2, bottom = pos.y + si.y / 2,
        left = pos.x - si.x / 2, right = pos.x + si.x / 2;
  /*  std::cout << top<<' '<<left << std::endl;*/
   /* std::cout << bottom << ' ' << right<< std::endl;*/
    rectangle((int)left, (int)top, (int)right, (int)bottom);
}

//得到重叠矩形
Vector2D BoxCollider::GetOverlapRect( Rect rect1, Rect rect2)
{
    return Vector2D(min(rect1.right,rect2.right)-max(rect1.left,rect2.left), min(rect1.bottom,rect2.bottom)-max(rect1.top,rect2.top));
}


/**
 * 初始化逻辑.
 * @brief 获取刚体组件的指针
 */
void Collider::BeginPlay()
{
    SceneComponent::BeginPlay();
    
    
    rigidAttached = GetOwner()->GetComponentByClass<RigidBody>();
   /* std::cout << GetName();*/
    
    if (rigidAttached)rigidAttached->colliders.insert(this);
}

/**
 * 处理析构处理不了的.
 * @brief 主要就是一些顺序和生命周期的问题
 * Debug 2024/11/11 析构函数的时候会先调用子类的析构再调用父类的析构函数，但是删除组件是在父类执行，访问不到子类的成员，也转换不成子类
 */
//Debug 2024/11/11 析构函数的时候会先调用子类的析构再调用父类的析构函数，但是删除组件是在父类执行，访问不到子类的成员，也转换不成子类
void Collider::EndPlay()
{
   
    if (rigidAttached)rigidAttached->colliders.erase(this); mainWorld.GameColliders.erase(this); Clear();
}

/**
 * 实现更新碰撞坐标, 进行容器的更新.
 * 
 */
void Collider::Update(float deltaTime)
{
    SceneComponent::Update(deltaTime);
    if (this->mode == ColliderMode::None || !bIsEnabled)return;
   
 
    Vector2D half = GetRect().GetSize() * 0.5f; 
  /*  Vector2D half ;
    if (shape == ColliderShape::Circle)
    {
        float r = Cast<CircleCollider>(this)->GetRadius();
        half = { r,r };
    }
    else if (shape == ColliderShape::Box)
    {
        half = Cast<BoxCollider>(this)->GetSize() / 2;
    }*/
    Vector2D pos = GetWorldPosition()-half;

    Pair new_point = {int( Math::Clamp(pos.x / 100, 0, RangeNumX-1)), int( Math::Clamp(pos.y / 100, 0, RangeNumY-1)) };
    pos += half * 2;
    Pair new_point_rb = { int(Math::Clamp(pos.x / 100, 0, RangeNumX-1)),  int(Math::Clamp(pos.y / 100, 0, RangeNumY-1) )};

    /*   碰撞区块更新    */
    //如果坐标没有更新就不用进行世界内部的容器增删
    if (new_point == point && new_point_rb == point_rb)return;
    //防止第一次出错
    if (point != Pair(-1, -1))for (int x = point.x; x <= point_rb.x; x++)for (int y = point.y; y <= point_rb.y; y++)mainWorld.ColliderZones[x][y].erase(this);
    point = new_point; point_rb = new_point_rb;
    for (int x = point.x; x <= point_rb.x; x++)for (int y = point.y; y <= point_rb.y; y++)mainWorld.ColliderZones[x][y].insert(this);



}

void Collider::Clear()
{
    
    for (auto& another : collisions)
    {
        another->collisions.erase(this);
        if (another->mode == ColliderMode::Collision && this->mode == ColliderMode::Collision)continue;
        OnComponentEndOverlap.BroadCast(this,another, another->pOwner);
        another->OnComponentEndOverlap.BroadCast(another,this, this->pOwner);
    }
    collisions.clear();
    if (point != Pair(-1, -1))
    {
        for (int i = point.x; i <= point_rb.x; ++i)for (int j = point.y; j <= point_rb.y; ++j)mainWorld.ColliderZones[i][j].erase(this);
    }
    point = { -1, -1 }, point_rb = { -1, -1 };
}


void Collider::Erase()
{

    
     this->collisions_to_erase.clear();
     for (auto& another : collisions)
     {
         
         if (!CollisionJudge(another))
         {
             another->collisions.erase(this); collisions_to_erase.push_back(another);
             //只有触发状态才执行委托
             if (this->GetCollisionMode() == ColliderMode::Collision && another->GetCollisionMode() == ColliderMode::Collision)continue;
             OnComponentEndOverlap.BroadCast(this,another, another->pOwner);
             another->OnComponentEndOverlap.BroadCast(another,this, this->pOwner);
         }
     }
     for (auto& another : collisions_to_erase)collisions.erase(another);
}

void Collider::Insert(Collider* another)
{
     
        //如果存储碰撞容器中没有，并且已经发生了碰撞
        if (mainWorld.collisionManager->ExistMapping(this->GetType(), another->GetType()) &&
            collisions.find(another) == collisions.end() &&
            CollisionJudge(another))
        {
            
            collisions.insert(another);
            another->collisions.insert(this);

            //如果都是碰撞模式进行物理模拟，不用触发
            if (this->GetCollisionMode() == ColliderMode::Collision && another->GetCollisionMode() == ColliderMode::Collision)
            {
                HitResult hitresult = CollisionHit(another);

                OnComponentHitOverlap.BroadCast(this, another, another->GetOwner(), -hitresult.impactnormalunit, hitresult);
                OnComponentHitOverlap.BroadCast(another, this, this->GetOwner(),hitresult.impactnormalunit, HitResult(hitresult.impactpoint,-hitresult.impactnormalunit, hitresult.hitobject, hitresult.hitcomponent));
            }
            else
            {
                OnComponentBeginOverlap.BroadCast(this, another, another->pOwner);
                another->OnComponentBeginOverlap.BroadCast(another, this, this->pOwner);
            }
        }
   
}

void Collider::Deactivate()
{
    SceneComponent::Deactivate();
   mainWorld.GameColliders_to_clear.insert(this); 
}

//圆形和圆形碰撞的物理信息
HitResult Collider::CollisionHitCircleToCircle(Collider*me, Collider*another)
{
    CircleCollider* circle1 = Cast<CircleCollider>(me);
    CircleCollider* circle2 = Cast<CircleCollider>(another);
    Vector2D impactnolunit =
        (me->GetWorldPosition() - another->GetWorldPosition()).unit();
    Vector2D impactpoint=me->GetWorldPosition()+impactnolunit*circle2->GetRadius();
    return HitResult(impactpoint,impactnolunit,circle2->GetOwner(),circle2);
}

//圆形和盒型碰撞的物理信息
HitResult Collider::CollisionHitCircleToBox(Collider* me, Collider* another)
{
    CircleCollider* circle;
    BoxCollider* box;
    if (me->GetShape() == ColliderShape::Circle)
    {
        circle = Cast<CircleCollider>(me); box = Cast<BoxCollider>(another);
    }
    else if (me->GetShape() == ColliderShape::Box)
    {
        circle = Cast<CircleCollider>(another); box = Cast<BoxCollider>(me);
    }
    else
    {
        circle = Cast<CircleCollider>(another); box = Cast<BoxCollider>(me);
    }
    Vector2D impactpoint;
    Vector2D impactnolunit;
    float radius = circle->GetRadius();
    Vector2D pos = circle->GetWorldPosition();
    Rect rec_pos = box->GetRect();
    //碰撞的速度太大，一次太里面
    if (pos.x <= rec_pos.right && pos.x >= rec_pos.left
        && pos.y <= rec_pos.bottom && pos.y >= rec_pos.top)
    {
        impactpoint = pos;      //不精确的返回
        impactnolunit = ((another->GetWorldPosition() - me->GetWorldPosition()).unit());
    }
    else
    {
        if (pos.x<rec_pos.left)
        {
            if (pos.y < rec_pos.top) { impactpoint = { rec_pos.left,rec_pos.top }, impactnolunit = (impactpoint - circle->GetWorldPosition()).unit(); }
            else if(pos.y>rec_pos.bottom){ impactpoint = { rec_pos.left,rec_pos.bottom }, impactnolunit = (impactpoint - circle->GetWorldPosition()).unit(); }
            else
            {
                impactpoint = { rec_pos.left,pos.y }, impactnolunit = {1,0};
            }
        }
        else if (pos.x > rec_pos.right)
        {
            if (pos.y < rec_pos.top) { impactpoint = { rec_pos.right,rec_pos.top }, impactnolunit = (impactpoint - circle->GetWorldPosition()).unit(); }
            else if (pos.y > rec_pos.bottom) { impactpoint = { rec_pos.right,rec_pos.bottom }, impactnolunit = (impactpoint - circle->GetWorldPosition()).unit(); }
            else
            {
                impactpoint = { rec_pos.right,pos.y }, impactnolunit = { -1,0 };
            }
        }
        else
        {
            if (pos.y < rec_pos.top)
            {
                impactpoint = { pos.x,rec_pos.top }, impactnolunit = { 0,1 };
            }
            else 
            {
                impactpoint = { pos.x,rec_pos.bottom }, impactnolunit = { 0,-1 };
            }
        }
    }
    return HitResult(impactpoint,impactnolunit*(me==circle?1.f:-1.f), another->GetOwner(), another);
}

//盒型和盒型碰撞的物理信息 其实是一个矩形，返回的是中心坐标
HitResult Collider::CollisionHitBoxToBox(Collider*  me, Collider* another)
{
    BoxCollider* box1 = Cast<BoxCollider>(me);
    BoxCollider* box2 = Cast<BoxCollider>(another);

    Vector2D overlaprect = BoxCollider::GetOverlapRect(box1->GetRect(), box2->GetRect());
    Vector2D impactnolunit;
    if (overlaprect.x > overlaprect.y)
    {
        //往上碰撞   往下碰撞
        box1->GetWorldPosition().y > box2->GetWorldPosition().y ? impactnolunit = { 0,-1 } : impactnolunit = {0,1};
    }
    else if (overlaprect.x < overlaprect.y)
    {
        //往上碰撞   往下碰撞
        box1->GetWorldPosition().x > box2->GetWorldPosition().x ? impactnolunit = { -1,0 } : impactnolunit = { 1,0 };
    }
    else
    {
        if (box1->GetWorldPosition().x > box2->GetWorldPosition().x )
        {
            box1->GetWorldPosition().y > box2->GetWorldPosition().y ? impactnolunit = Vector2D(-1, -1).unit() : impactnolunit = Vector2D(-1, 1).unit();

        }
        else
        {
            box1->GetWorldPosition().y > box2->GetWorldPosition().y ? impactnolunit = Vector2D(1, -1).unit() : impactnolunit = Vector2D(1, 1).unit();
        }
    }
    float x = max(box1->GetRect().left, box2->GetRect().left) + overlaprect.x / 2;
    float y = max(box1->GetRect().bottom, box2->GetRect().bottom) + overlaprect.y / 2;
    return HitResult({x,y},impactnolunit,box2->GetOwner(),box2);
}

//碰撞映射 进行选择哪个函数
HitResult Collider::CollisionHit(Collider* another)
{
    return CollisionHitMap[int(this->GetShape()) + int(another->GetShape())](this,another);
}

