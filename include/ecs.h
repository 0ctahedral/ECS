#pragma once
#include "defines.h"
#include "components.h"
#include "compstore.h"
#include <assert.h>
#include <algorithm>
#include <typeinfo>
#include <type_traits>
#include <deque>
#include <vector>
#include <bitset>


// T is the base class. We need to do this to set the static s_Types variable.
// we also use it to make sure that the types given are derived from T
// May be useful if we need to do this with things other than components

/// Helper to generate Component type ids
template<class T>
class IdGenerator {
  private:
    /// The number of types we have
    static u8 s_Types;

  public:
    /// Creates a unique numerical representation of a type that is derived from Component
    /// @return a number representing the type given
    template<typename C>
    static const u8 TypeId() {
      // create a new type if we can
      static const u8 tid = s_Types++;
      assert(tid <= MAX_COMPONENTS && "Maximum component types exceeded");
      return tid;
    };
};

template<class T> u8 IdGenerator<T>::s_Types = 0;
/// Entity Component Manager
class ECS {
  private:

    /// Stores ids of unused entities
    std::deque<u32> m_EntityIds;

    /// The number of living entities right now
    u32 m_LivingEntities;

    /// Vector of component storage
    std::vector<Scope<CompStoreBase>> m_CompStorage;

    /// Array of bitmaps to signify which components an entity has
    //std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITIES> m_CompSet{};
    std::array<u64, MAX_ENTITIES> m_CompSet{};

  public:
    /// Constructor
    ECS(){ 
      // populate the queue
      // id of 0 is reserved for null
      for (int i = 1; i < MAX_ENTITIES; ++i) {
        m_EntityIds.push_back(i);
      }
    };


    /// Destructor
    ~ECS() {};

    /// Creates a new entity
    /// @return a fresh new unused EntityId
    u32 CreateEntity() {
      // make sure we can even create a new entity
      assert(m_LivingEntities < MAX_ENTITIES && "Max entities exceeded");
      // set our return value to the frontmost entity id
      u32 ret = m_EntityIds.front();
      // pop the id off the front
      m_EntityIds.pop_front();
      // increase number of living entities
      ++m_LivingEntities;
      return ret;
    }


    /// Get a list of entity ids that have the same components
    /// @return a vector of the entity id's we want
    template<typename... Cs>
    std::vector<u32> GetEntitiesWith() {
      // the set we are going to populate
      //std::bitset<MAX_COMPONENTS> set{0};
      u64 set = 0;
      // reify the list of component types given to use
      for (u8 tid : std::vector<u8>{{IdGenerator<Component>::TypeId<Cs>()...}}) {
        set |= (1 << tid);
      }
      /// the list of ids to return
      std::vector<u32> ret;
      // generate a bitset from types specified
      for (int i = 0; i < MAX_COMPONENTS; ++i) {
        if ((m_CompSet[i] & set) == set) {
          ret.push_back(i);
        }
      }

      return ret;
    }

    /// Destroys and entity
    /// @param id of entity that we no longer need anymore
    void DestroyEntity(u32 id) {
      assert(id < MAX_ENTITIES && "Invalid entity id given");
      // check if this entity has already been returned
      auto itr = std::find(m_EntityIds.begin(), m_EntityIds.end(), id);
      if (itr != m_EntityIds.end()) {
        return;
      }
      // remove all the entity's components
      for (auto& itr : m_CompStorage) {
        itr->EntityDestroyed(id);
      }
      // add to back of list
      m_EntityIds.push_back(id);
      // reduce number of living entities
      --m_LivingEntities;
    }


    /// Creates and adds a component to a given entity
    /// @param id of the entity we want to add the component to
    /// @param args arguments to the constructor of that component
    template<typename C, typename... Args>
    void AddComponent(u32 id, Args&&... args) {
      auto cs = getComponentStore<C>();
      cs->Add(id, std::forward<Args>(args)...);
      // add to bitset
      u8 tid = IdGenerator<Component>::TypeId<C>();
      //m_CompSet[id].set(tid, true);
      m_CompSet[id] |= (1 << tid);
    }


    /// removes a component from an entity
    /// @param id of the entity we want to remove the component from
    template<typename C>
    void RemoveComponent(u32 id) {
      auto cs = getComponentStore<C>();
      cs->Remove(id);
      // remove from bitset
      u8 tid = IdGenerator<Component>::TypeId<C>();
      //m_CompSet[id].set(tid, false);
      m_CompSet[id] &= ~(1 << tid);
    }


    /// Returns a pointer to a component owned by this entity
    /// @param id of the entity we want the component of
    template<typename C>
    C* GetComponent(u32 id) {
      auto cs = getComponentStore<C>();
      return cs->Get(id);
    }

  private:
    /// Helper for creating or finding an component store
    /// @return the component store
    template<typename C>
    CompStore<C>* getComponentStore() {
      u8 tid = IdGenerator<Component>::TypeId<C>();

      // if the length of the comp storage vector is less than our id then that means we gotta add it
      if (m_CompStorage.size() <= tid) {
        m_CompStorage.push_back(CreateScope<CompStore<C>>());
      }

      return  (CompStore<C>*) m_CompStorage[tid].get();
    }

};
