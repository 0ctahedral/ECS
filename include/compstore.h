#pragma once
#include "defines.h"
#include "components.h"
#include <string.h>

/// Virtual class for which component storage is derived
class CompStoreBase {
  public:
    /// Creates a component storage container
    CompStoreBase() {};

    /// Virtual destructor
    virtual ~CompStoreBase() {};

    /// Notify the component store that an entity was destroyed
    /// @param id of the entity that is being destroyed
    virtual void EntityDestroyed(u32 id) = 0;
};

/// Template class for component storage
/// This class stores components packed in memory so that we maintian
/// cache locality
template<typename C>
class CompStore : public CompStoreBase {
  private:
    /// storage
    std::array<C, MAX_ENTITIES> m_Store;

    /// get the storage index of a component based on the entity id
    std::array<u32, MAX_ENTITIES> m_Id2Idx{0};

    /// get the Id of an entity based on the index of its component
    std::array<u32, MAX_ENTITIES> m_Idx2Id{0};

    /// the index of the last used component
    u32 m_LastIdx{1};

  public:
    /// Constructor
    CompStore() { };
    /// Destructor, override the virtual
    ~CompStore() override { }


    /// Create a new component
    /// @param id the id of the entity to add
    /// @param args the arguments to the constructor of the entity
    template<typename... Args>
      void Add(u32 id, Args&&... args) {
        // store the component in the closest available spot
        m_Store[m_LastIdx] = C(args...);
        // store the index of the component data at the index of the id
        m_Id2Idx[id] = m_LastIdx;
        m_Idx2Id[m_LastIdx] = id;
        // increase the last index
        ++m_LastIdx;
      }

    /// Remove a component
    /// @param id the id of the entity to remove from
    void Remove(u32 id) {
      // decrease the last idx counter so it "points" to the last item
      --m_LastIdx;

      // if this is the last added component
      if (m_Id2Idx[id] == m_LastIdx) {
        // we can just remove it from the end and erase
        memset(&m_Store[m_LastIdx], 0, sizeof(C));
        // reset to -1 for no component
        m_Id2Idx[id] = 0;
        m_Idx2Id[m_LastIdx] = 0;
        return;
      }
      // new place is the index of the id we are removing
      u32 new_idx = m_Id2Idx[id];
      // update the idx of this index to null
      m_Id2Idx[id] = 0;

      // first get the id of the last component
      u32 last_id = m_Idx2Id[m_LastIdx];
      // update the arrays
      m_Id2Idx[last_id] = new_idx;
      m_Idx2Id[new_idx] = last_id;
      m_Idx2Id[m_LastIdx] = 0;

      // move the component
      m_Store[new_idx] = m_Store[m_LastIdx];

      // clear the memory
      memset(&m_Store[m_LastIdx], 0, sizeof(C));
    }

    /// Get a reference to the component for this entity
    /// @param id of the entity whos component we are getting
    C* Get(u32 id) {
      // get index
      u32 idx = m_Id2Idx[id];
      if (idx == 0)
        return nullptr;
      // return reference
      return &m_Store[idx];
    }

    void EntityDestroyed(u32 id) override {
      Remove(id);
    }
};
