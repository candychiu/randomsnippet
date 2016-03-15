
#include <memory>
#include <unordered_set>

template<class Entity, class Id>
using IdFunc =
    typename std::function<const Id& (const Entity&)>;

template<class Entity, class Id>
struct IdEqualTo {
    explicit IdEqualTo(const IdFunc<Entity, Id>& idFunc) : idFunc_(idFunc) {}
    bool operator()(const Entity& lhs, const Entity& rhs) const {
        return idFunc_(lhs) == idFunc_(rhs);
    }
private:
    IdFunc<Entity, Id> idFunc_;
};
     
// specialization for pointer types
template<class Entity, class Id>
struct IdEqualTo<std::unique_ptr<Entity>, Id> {
    explicit IdEqualTo(const IdFunc<Entity, Id>& idFunc) : idFunc_(idFunc) {}
    typedef std::unique_ptr<Entity> PtrTy;
    bool operator()(const PtrTy& lhs, const PtrTy& rhs) const {
        return idFunc_(*lhs) == idFunc_(*rhs);
    }      
private:
    IdFunc<Entity, Id> idFunc_;   
};

template<class Entity, class Id>
struct IdEqualTo<std::shared_ptr<Entity>, Id> {
    explicit IdEqualTo(const IdFunc<Entity, Id>& idFunc) : idFunc_(idFunc) {}
    typedef std::shared_ptr<Entity> PtrTy;
    bool operator()(const PtrTy& lhs, const PtrTy& rhs) const {
        return idFunc_(*lhs) == idFunc_(*rhs);
    }      
private:
    IdFunc<Entity, Id> idFunc_;   
};

template<class Entity, class Id>
struct IdHash {
public:    
    explicit IdHash(const IdFunc<Entity, Id>& idFunc) : idFunc_(idFunc), hash_() {}
    inline std::size_t operator()(const Entity& o) const {
        return hash_(idFunc_(o));
    }  
    inline std::size_t operator()(const std::unique_ptr<Entity>& o) const {
        return (*this)(*o);
    }   
    inline std::size_t operator()(const std::shared_ptr<Entity>& o) const {
        return (*this)(*o);
    }     
private:
    IdFunc<Entity, Id> idFunc_;
    std::hash<Id> hash_;
};    

struct MyClass {
	const std::string& id() const { return id_; }
	std::string id_;
};

struct Client1 {    
public:
    typedef std::shared_ptr<MyClass> SharedPtr;
    typedef std::unordered_set<std::shared_ptr<MyClass>, 
        IdHash<MyClass, std::string>, IdEqualTo<SharedPtr, std::string>> SetTy;
		
    Client1() 
	: shared_{ 0, 
           IdHash<MyClass, std::string>(&MyClass::id), 
           IdEqualTo<SharedPtr, std::string>(&MyClass::id) }
    {}
    
private:        
    SetTy shared_;    
};

struct Client2 {    
public:
    typedef std::unique_ptr<MyClass> UniquePtr;
    typedef std::unordered_set<std::unique_ptr<MyClass>, 
        IdHash<MyClass, std::string>, IdEqualTo<UniquePtr, std::string>> SetTy;
		
    Client2() 
	: owned_ { 0, 
           IdHash<MyClass, std::string>(&MyClass::id), 
           IdEqualTo<UniquePtr, std::string>(&MyClass::id) }
    {}
    
private:        
    SetTy owned_;
};

struct Client3 {    
public:
    typedef std::unordered_set<MyClass, 
        IdHash<MyClass, std::string>, IdEqualTo<MyClass, std::string>> SetTy;
		
    Client3() 
	: owned_ { 0, 
           IdHash<MyClass, std::string>(&MyClass::id), 
           IdEqualTo<MyClass, std::string>(&MyClass::id) }
    {}
    
private:        
    SetTy owned_;
};

int main() {
	Client1 client1;
	Client2 client2;
	Client3 client3;
	return 0;
}