# Test case

It is a basic producer/consumer test. There are : 
* N thread(s) create item with object pool,
* M thread(s) release item.

The communication between producer and consumer is done via queue and item size is 10240 octets.

# What is measured ?

* item's allocation (min/max/avg)
* item's release (min/max/avg)
* time to process the benchmark
* memory

# Algorithm

* none : acquire call new and when object is not need it is destroyed  
* mutex + list : a list of free objects is created protected by a mutex

  * acquire need to lock mutex and check list if there is no available item, a new item is created
  * release need to lock mutex and add item in the list

  ```c++
  ptr acquire( ARGS ... args)
  {
     lock_guard guard(mutex_);    
     if(free_item.empty())
     {
       return get_free_item_(args...);
     }
     else
     {
       return create_new_item_(args...);
     }
  }
  ```

* segment linear : it is a one way chain list of segment. A segment is a simple memory pool but each memory block are always same size return nullptr if allocation is not possible. If allocate is not possible, create next segment if not exist and continue on the next segment 

  ```c++
  struct node
  {
    segment segment_;
    node*   next_;
  };

  node root_

  ptr acquire( ARGS ... args)
  {
    return acquire_(root_, args...);
  }

  ptr acquire_(node &n, ARGS ... args)
  {
    void* data = n.segment_.acquire();
    if(data)
    {
      return create_ptr_(data, args...);
    }
    else
    {
      if(!n.next_)
      {
        n.next_ = new node();
      }
      return acquire_(*n.next_, args...);
    }    
  }
  ```
  
  _segment linear_ is full lockfree (acquire / release) and support multi acquire and release in same time.
 
* segment tree (old and v2) : it is very closed to _segment linear_ is is a tree of segment

  ```c++
  struct node
  {
    segment segment_;
    node*   childs_[64];
  };

  node root_

  ptr acquire( ARGS ... args)
  {
    return acquire_(root_, args...);
  }

  ptr acquire_(node &n, ARGS ... args)
  {
    void* data = n.segment_.acquire();
    if(data)
    {
      return create_ptr_(data, args...);
    }
    else
    {
      node* next = find_next_child_(n);
      return acquire_(*next, args...);
    }    
  }
  ```

  the main difference between the two version if _segment tree_ is the predicate to find the best child :
  * old version, it is sequential 

  ```c++

  struct node
  {
    segment segment_;
    node*   childs_[64];
    int     last_acquire_id_; // value 0 <-> 63
  };

  node* find_next_child_(node& n)
  {
     int id = n.last_acquire_id_;
     n.last_acquire_id_ = (n.last_acquire_id_ + 1) % 64;
     if(!n.child_[id])
     {
       n.child_[id] = new node();
     }
     return n.child_[id];
  }

  ```

  * new version, the predicate try to find child with free element(s) otherwise child with minimum child

  ```c++

  struct node
  {
    segment segment_;
    node*   childs_[64];

    // return nb free item on the segment + nb free element on each child
    int size() 
    {
      int ret = segment_.size();
      for(node* child : childs_)
      {
        if(child) {ret += child->size();}
      }
      return ret;
    }

    // return nb childs
    int get_nb_child()
    {
      int ret = 0;
      for(node* child : childs_)
      {
        if(child) {ret += 1 + child->get_nb_child();}
      }
      return ret;
    }
  };

  node* find_next_child_(node& n)
  {
     for(node* child : n.childs_)
     {
       if(child && (child -> size() != 0))
       {
          return child;
       }
     }

     int id = 0;
     int id_min = 0;
     int nb_min_child = +inf;
     for(node* child : n.childs_)
     {
       if(child)
       {
         int nb_child = child.get_nb_child();
         if(nb_child < nb_min_child)
         {
           id_min       = id;
           nb_min_child = nb_child ;
         }
       }
       else
       {
         id_min       = id;
         nb_min_child = 0;
       }
       ++id;
     }

     node *& next = n.childs_[id];
     if(!next)
     {
       next = new node();
     }
     return next;
  }

  ```

  _segment tree_ is full lockfree (acquire / release) and support multi acquire and release in same time.

# Comparaison

## Variables

Nb producer (N) | Nb consumer (M)
----------------| ---------------
1               | 30
30              |  1
15              | 15

## Which computer ?

* linux (Red Hat 6.7)
* g++ (4.8.2-15)
* 2 processors (_Intel(R) Xeon(R) CPU E5-2667 v2 @ 3.30GHz_) 8 cores
* 32 Go memory

## global

### N=30 and M=1

algorithm  | Nb Items by Producer | Time to process (Ms) | Nb items in pool
-----------|----------------------|----------------------|-----------------
none       |               100000 |                40601 | 0
mutex+list |               100000 |                34768 | 2636101
linear     |               100000 |                 6607 | 50368
tree (old) |               100000 |                 4108 | 5153856
tree (new) |               100000 |                 3879 | 309696


### N=15 and M=15

algorithm  | Nb Items by Producer | Time to process (Ms) | Nb items in pool
-----------|----------------------|----------------------|-----------------
none       |               100000 |                 3402 | 0
mutex+list |               100000 |                 2737 | 284
linear     |               100000 |                 2332 | 2176
tree (old) |               100000 |                 2285 | 4160
tree (new) |               100000 |                 2269 | 2880


### N=1 and M=30

algorithm  | Nb Items by Producer | Time to process (Ms) | Nb items in pool
-----------|----------------------|----------------------|-----------------
none       |               100000 |                 2029 | 0
mutex+list |               100000 |                 2058 | 13
linear     |               100000 |                 1905 | 64
tree (old) |               100000 |                 1970 | 64
tree (new) |               100000 |                 2011 | 64

## memory allocation

### N=30 and M=1

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |       13921 | 373
mutex+list |           0 |       16841 | 332
linear     |           0 |       12125 | 33
tree (old) |           0 |       12726 | 23
tree (new) |           0 |       14984 | 14

![How allocation is done in less ... micro sec ?](./images/perf_object_pool_01/alloc_30_1_global.png)

![How allocation is done in less ... micro sec ? (zoom on 0 - 55 micro)](./images/perf_object_pool_01/alloc_30_1_zoom.png)

### N=15 and M=15

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |      172928 | 3
mutex+list |           0 |       14559 | 13
linear     |           0 |         656 | 1
tree (old) |           0 |         657 | 0
tree (new) |           0 |         554 | 1

### N=1 and M=30

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |        2036 | 2
mutex+list |           0 |       22725 | 4
linear     |           0 |        2955 | 0
tree (old) |           0 |        1307 | 0
tree (new) |           0 |        1099 | 0

## memory free

### N=30 and M=1

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |       13921 | 373
mutex+list |           0 |       11659 | 11
linear     |           0 |          16 | 0
tree (old) |           0 |         667 | 0
tree (new) |           0 |        3611 | 0

### N=15 and M=15

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |      172928 | 3
mutex+list |           0 |       11843 | 11
linear     |           0 |         840 | 0
tree (old) |           0 |         840 | 0
tree (new) |           0 |         611 | 0

### N=1 and M=30

algorithm  | Min (micro) | Max (micro) | Avg (Micro)
-----------|-------------|-------------|------------
none       |           0 |        2036 | 2
mutex+list |           0 |        1615 | 5
linear     |           0 |          89 | 0
tree (old) |           0 |        1307 | 0
tree (new) |           0 |        1099 | 0

# Conclusion
# Links
[source] (../tests/perf_object_pool_01.cc)