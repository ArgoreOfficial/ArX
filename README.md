# unordered_array

### Features
* Continuous block of memory
* Keys are **always** valid, until erased

# Usage
```cpp
typedef uint16_t ID;

arg::unordered_array<ID, SomeObject> objects;
ID id1 = objects.emplace( "object1", 0, 1.0f  );
ID id2 = objects.emplace( "object2", 4, 0.42f );
```
```cpp
objects.erase( id1 );
id1 = 0; // make sure to invalidate key
```

Accessing the real object can be done using `at()`

```cpp
SomeObject& obj2 = objects.at( id2 );
obj2.name = "coolerObject2";
```

```cpp
objects.size();  // true size in bytes of the container
objects.count(); // number of currently allocated elements in the container
```

## Note on multithreading
Just like other continous-memory-containers, the memory buffer may be reallocated whenever a new object is emplaced. This will invalidate any pointers or current references to said object  
aka; **use mutexes** as you would with a vector  
Keys will always remain valid 