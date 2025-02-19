# libWyvern - Common Library for the Wyvern Game Engine
classes and functions I use in several projects, all combined in one project!

### README TODO AND SUBJECT TO CHANGE


# Notable Features
## unordered_array

Continuous container with keys that do not become invalid upon reallocation.  
It's essentially a hybrid of `std::vector` and `std::unordered_map`, and similar to how OpenGL Object IDs are created.

Note: Keys must be integers or castable to `size_t`
```cpp
typedef uint16_t ID;
struct Foo
{
    const char* name;
    int bar;
};

wv::unordered_array<ID, Foo> objects;
ID id1 = objects.emplace( "foo1", 42 );
ID id2 = objects.emplace( "foo2", 11 );

objects.erase( id1 );
id1 = 0; // 0 is considered invalid handle, much like OpenGL

objects.at( id2 ).name = "coolfoo";
objects[ id2 ].bar = 9000;
```

## Note on multithreading
Just like other continous-memory-containers, the memory buffer may be reallocated whenever a new object is emplaced. This will invalidate any pointers or references to said object  
aka; **use mutexes** as you would with a vector  
Keys will always remain valid 
