//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Attributable entities contain one of these, which handles game specific handling:
//				- Save / Restore
//				- Networking
//				- Attribute providers
//				- Application of attribute effects
//
//=============================================================================
#include "SignatureManager.h"
#include "SDK.h"
#ifndef ATTRIBUTE_MANAGER_H
#define ATTRIBUTE_MANAGER_H
#ifdef _WIN32
#pragma once
#endif
template< class T, class A = CUtlMemory<T> >
class CUtlVector
{
	typedef A CAllocator;
public:
	typedef T ElemType_t;
	typedef T* iterator;
	typedef const T* const_iterator;

	// Set the growth policy and initial capacity. Count will always be zero. This is different from std::vector
	// where the constructor sets count as well as capacity.
	// growSize of zero implies the default growth pattern which is exponential.
	explicit CUtlVector(int growSize = 0, int initialCapacity = 0);

	// Initialize with separately allocated buffer, setting the capacity and count.
	// The container will not be growable.
	CUtlVector(T* pMemory, int initialCapacity, int initialCount = 0);
	~CUtlVector();

	// Copy the array.
	CUtlVector<T, A>& operator=(const CUtlVector<T, A>& other);

	// element access
	T& operator[](int i);
	const T& operator[](int i) const;
	T& Element(int i);
	const T& Element(int i) const;
	T& Head();
	const T& Head() const;
	T& Tail();
	const T& Tail() const;
	T& Random();
	const T& Random() const;

	// STL compatible member functions. These allow easier use of std::sort
	// and they are forward compatible with the C++ 11 range-based for loops.
	iterator begin() { return Base(); }
	const_iterator begin() const { return Base(); }
	iterator end() { return Base() + Count(); }
	const_iterator end() const { return Base() + Count(); }

	// Gets the base address (can change when adding elements!)
	T* Base() { return m_Memory.Base(); }
	const T* Base() const { return m_Memory.Base(); }

	// Returns the number of elements in the vector
	// SIZE IS DEPRECATED!
	int Count() const;
	int Size() const;	// don't use me!

	/// are there no elements? For compatibility with lists.
	inline bool IsEmpty(void) const
	{
		return (Count() == 0);
	}

	// Is element index valid?
	bool IsValidIndex(int i) const;
	static int InvalidIndex();

	// Adds an element, uses default constructor
	int AddToHead();
	int AddToTail();
	T* AddToTailGetPtr();
	int InsertBefore(int elem);
	int InsertAfter(int elem);

	// Adds an element, uses copy constructor
	int AddToHead(const T& src);
	int AddToTail(const T& src);
	int InsertBefore(int elem, const T& src);
	int InsertAfter(int elem, const T& src);

	// Adds multiple elements, uses default constructor
	int AddMultipleToHead(int num);
	int AddMultipleToTail(int num);
	int AddMultipleToTail(int num, const T* pToCopy);
	int InsertMultipleBefore(int elem, int num);
	int InsertMultipleBefore(int elem, int num, const T* pToCopy);
	int InsertMultipleAfter(int elem, int num);

	// Calls RemoveAll() then AddMultipleToTail.
	// SetSize is a synonym for SetCount
	void SetSize(int size);
	// SetCount deletes the previous contents of the container and sets the
	// container to have this many elements.
	// Use GetCount to retrieve the current count.
	void SetCount(int count);
	void SetCountNonDestructively(int count); //sets count by adding or removing elements to tail TODO: This should probably be the default behavior for SetCount

	// Calls SetSize and copies each element.
	void CopyArray(const T* pArray, int size);

	// Fast swap
	void Swap(CUtlVector< T, A >& vec);

	// Add the specified array to the tail.
	int AddVectorToTail(CUtlVector<T, A> const& src);

	// Finds an element (element needs operator== defined)
	int Find(const T& src) const;

	// Helper to find using std::find_if with a predicate
	//   e.g. [] -> bool ( T &a ) { return a.IsTheThingIWant(); }
	//
	// Useful if your object doesn't define a ==
	template < typename F >
	int FindPredicate(F&& predicate) const;

	void FillWithValue(const T& src);

	bool HasElement(const T& src) const;

	// Makes sure we have enough memory allocated to store a requested # of elements
	// Use NumAllocated() to retrieve the current capacity.
	void EnsureCapacity(int num);

	// Makes sure we have at least this many elements
	// Use GetCount to retrieve the current count.
	void EnsureCount(int num);

	// Element removal
	void FastRemove(int elem);	// doesn't preserve order
	void Remove(int elem);		// preserves order, shifts elements
	bool FindAndRemove(const T& src);	// removes first occurrence of src, preserves order, shifts elements
	bool FindAndFastRemove(const T& src);	// removes first occurrence of src, doesn't preserve order
	void RemoveMultiple(int elem, int num);	// preserves order, shifts elements
	void RemoveMultipleFromHead(int num); // removes num elements from tail
	void RemoveMultipleFromTail(int num); // removes num elements from tail
	void RemoveAll();				// doesn't deallocate memory

	// Memory deallocation
	void Purge();

	// Purges the list and calls delete on each element in it.
	void PurgeAndDeleteElements();

	// Compacts the vector to the number of elements actually in use 
	void Compact();

	// Set the size by which it grows when it needs to allocate more memory.
	void SetGrowSize(int size) { m_Memory.SetGrowSize(size); }

	int NumAllocated() const;	// Only use this if you really know what you're doing!

	void Sort(int(__cdecl* pfnCompare)(const T*, const T*));

	void Shuffle(DWORD* pSteam = NULL);

	// Call this to quickly sort non-contiguously allocated vectors
	void InPlaceQuickSort(int(__cdecl* pfnCompare)(const T*, const T*));
	// reverse the order of elements
	void Reverse();

#ifdef DBGFLAG_VALIDATE
	void Validate(CValidator& validator, char* pchName);		// Validate our internal structures
#endif // DBGFLAG_VALIDATE

	/// sort using std:: and expecting a "<" function to be defined for the type
	void Sort(void);

	/// sort using std:: with a predicate. e.g. [] -> bool ( T &a, T &b ) { return a < b; }
	template <class F> void SortPredicate(F&& predicate);

protected:
	// Can't copy this unless we explicitly do it!
	CUtlVector(CUtlVector const& vec) { assert(0); }

	// Grows the vector
	void GrowVector(int num = 1);

	// Shifts elements....
	void ShiftElementsRight(int elem, int num = 1);
	void ShiftElementsLeft(int elem, int num = 1);

	CAllocator m_Memory;
	int m_Size;

#ifndef _X360
	// For easier access to the elements through the debugger
	// it's in release builds so this can be used in libraries correctly
	T* m_pElements;

	inline void ResetDbgInfo()
	{
		m_pElements = Base();
	}
#else
	inline void ResetDbgInfo() {}
#endif

private:
	void InPlaceQuickSort_r(int(__cdecl* pfnCompare)(const T*, const T*), int nLeft, int nRight);
};


#define CALL_ATTRIB_HOOK(vartype, retval, hookName, who, itemlist) \
		retval = AttribHookValue<vartype>(retval, #hookName, static_cast<const CBaseEntity*>(who), itemlist, true);

#define CALL_ATTRIB_HOOK_INT(retval, hookName) CALL_ATTRIB_HOOK(int, retval, hookName, this, NULL)
#define CALL_ATTRIB_HOOK_FLOAT(retval, hookName) CALL_ATTRIB_HOOK(float, retval, hookName, this, NULL)
#define CALL_ATTRIB_HOOK_INT_ON_OTHER( other, retval, hookName )	CALL_ATTRIB_HOOK( int, retval, hookName, other, NULL )
#define CALL_ATTRIB_HOOK_FLOAT_ON_OTHER( other, retval, hookName )	CALL_ATTRIB_HOOK( float, retval, hookName, other, NULL )

template <class T> static T AttribHookValue(T TValue, const char* pszAttribHook, const CBaseEntity* pEntity, CUtlVector<CBaseEntity*>* pItemList = NULL, bool bIsGlobalConstString = false)
{
	//return reinterpret_cast<T(__cdecl*)(T, const char*, const CBaseEntity*, CUtlVector<CBaseEntity*>*, bool)>(*reinterpret_cast<DWORD*>(gSignatures.dwAttribHookValue) + gSignatures.dwAttribHookValue + 0x4)(TValue, pszAttribHook, pEntity, pItemList, bIsGlobalConstString);
}

#endif // ATTRIBUTE_MANAGER_H