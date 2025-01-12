# ArX - Argore Cross-Project Library
classes and functions I use in several projects, all combined in one project!

README TODO


# arx::unordered_array

### Why?

I needed a container that met these specific requirements
* Must have a continuous block of memory
* Keys/indices must **only** be invalidated when the user calls `erase()` or `clear()`
* non-size_t keys, eg. uint16_t

It's essentially a hybrid of `std::vector` and `std::unordered_map`  

# Usage
Note: Keys must be integer values, or castable to `size_t`
```cpp
typedef uint16_t ID;
struct SomeObject 
{
    const char* name;
    int coolness;
};


arx::unordered_array<ID, SomeObject> objects;
ID id1 = objects.emplace( "object1", 42 );
ID id2 = objects.emplace( "object2", 11 );

objects.erase( id1 );
id1 = 0; // make sure to invalidate key

objects.at( id2 ).name = "coolerObject2";
objects[ id2 ].coolness = 9000;
```

```cpp
objects.size();  // element size of buffer (size in bytes/size of element)
objects.count(); // number of currently allocated elements in the array
```

## Note on multithreading
Just like other continous-memory-containers, the memory buffer may be reallocated whenever a new object is emplaced. This will invalidate any pointers or references to said object  
aka; **use mutexes** as you would with a vector  
Keys will always remain valid 
