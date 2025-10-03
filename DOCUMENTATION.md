# Order-Statistic Tree: Size Attribute Maintenance

## Overview

This document explains how the **size attribute** is maintained in Order-Statistic Trees during insertion and deletion operations. The size attribute stores the number of nodes in the subtree rooted at each node, enabling O(log n) rank queries.

## Key Property

For any node `x` in the tree:
```
x.size = x.left.size + x.right.size + 1
```

Where `NULL` nodes have size 0.

---

## OS-TREE-INSERT: Maintaining Size During Insertion

### Algorithm Description

When inserting a new node `z` into the tree, we must increment the size of every node along the path from the root to the insertion point. This is because `z` will become part of the subtree rooted at each of these nodes.

### Pseudocode

```
OS-TREE-INSERT(T, z)
1   y = NIL
2   x = T.root
3
4   // Find insertion point and update sizes along the path
5   while x ≠ NIL
6       y = x
7       x.size = x.size + 1              // INCREMENT size for each node on path
8       if z.key < x.key
9           x = x.left
10      else
11          x = x.right
12
13  // Standard BST insertion
14  z.p = y
15  if y == NIL
16      T.root = z                       // Tree was empty
17  else if z.key < y.key
18      y.left = z
19  else
20      y.right = z
21
22  z.size = 1                           // New node has size 1
```

### Explanation

**Lines 5-11:** As we traverse down the tree searching for the insertion point, we increment the size of each node we visit (line 7). This is correct because:
- The new node `z` will be inserted somewhere in the subtree rooted at `x`
- Therefore `x.size` must increase by 1 to account for the new node

**Lines 14-22:** Standard BST insertion logic. The new node `z` is initialized with `size = 1` (itself only).

**Time Complexity:** O(h) where h is the height of the tree. For balanced BSTs, h = O(log n).

### Example

Inserting key 15 into a tree:

```
Before insertion:           After insertion:
    10(3)                      10(4)          <- size incremented
    /   \                      /   \
  5(1) 20(1)                5(1) 20(2)       <- size incremented
                                  /
                               15(1)          <- new node
```

---

## OS-TREE-DELETE: Maintaining Size During Deletion

### Algorithm Description

When deleting a node `z` from the tree, we must decrement the size of every node along the path from the root to `z`. Additionally, when handling the two-child case where we replace `z` with its successor `y`, we must also update sizes along the path from `z` to `y`.

### Pseudocode

```
OS-TREE-DELETE(T, z)
1   // Phase 1: Decrement size along path from root to z
2   current = T.root
3   while current ≠ NIL
4       current.size = current.size - 1      // DECREMENT size
5       if z.key < current.key
6           current = current.left
7       else if z.key > current.key
8           current = current.right
9       else
10          break                            // Found z
11
12  // Phase 2: Standard BST deletion with size updates
13  if z.left == NIL
14      TRANSPLANT(T, z, z.right)
15  else if z.right == NIL
16      TRANSPLANT(T, z, z.left)
17  else
18      y = TREE-MINIMUM(z.right)           // Successor
19
20      // If successor is not direct child, update path from y to z
21      if y.p ≠ z
22          temp = y.p
23          while temp ≠ z
24              temp.size = temp.size - 1    // DECREMENT along path
25              temp = temp.p
26
27          TRANSPLANT(T, y, y.right)
28          y.right = z.right
29          y.right.p = y
30
31      TRANSPLANT(T, z, y)
32      y.left = z.left
33      y.left.p = y
34
35      // Recompute y's size based on its new children
36      y.size = SIZE(y.left) + SIZE(y.right) + 1
```

### Explanation

**Phase 1 (Lines 2-10):** We traverse from the root to `z`, decrementing the size of each node. This is correct because:
- Node `z` is being removed from the subtree of each node on this path
- Therefore each node's size must decrease by 1

**Phase 2 - Case 1 (Lines 13-14):** If `z` has only a right child, we transplant it. No additional size updates needed because Phase 1 already handled the path.

**Phase 2 - Case 2 (Lines 15-16):** If `z` has only a left child, we transplant it. No additional size updates needed.

**Phase 2 - Case 3 (Lines 17-36):** If `z` has two children:
1. Find successor `y` (minimum of right subtree)
2. **Lines 21-25:** If `y` is not a direct child of `z`, we must decrement sizes along the path from `y.p` to `z` because `y` is moving up the tree
3. **Line 36:** Recompute `y.size` based on its new position (replacing `z`)

### Example 1: Deleting Leaf Node

Deleting key 5:
```
Before deletion:           After deletion:
    10(4)                      10(3)          <- size decremented
    /   \                      /   \
  5(1) 20(2)                NIL  20(2)
         /
      15(1)
```

### Example 2: Deleting Node with Two Children

Deleting key 10 (root):
```
Before:                    After:
    10(4)                      15(3)          <- successor moved up, size recomputed
    /   \                      /   \
  5(1) 20(2)                5(1) 20(1)       <- size decremented (on path from 15 to 10)
         /
      15(1)
```

**Path update:** When moving successor 15 up, we decrement 20's size because 15 is leaving 20's subtree.

---

## Why This Works

### Invariant Preservation

The key invariant is: `x.size = SIZE(x.left) + SIZE(x.right) + 1`

**INSERT maintains invariant:**
- By incrementing along the insertion path, we ensure every ancestor's size correctly reflects the new node
- The new node starts with size 1 (correct for a leaf)

**DELETE maintains invariant:**
- Phase 1 decrements along the path to `z`, removing `z` from ancestor counts
- In the two-child case, we handle the successor movement by:
  - Decrementing nodes along the path from successor to `z` (nodes losing the successor)
  - Recomputing the successor's size based on its new children

### Verification

After any sequence of insertions and deletions:
- Each node's size equals the number of nodes in its subtree
- OS-SELECT and OS-RANK rely on this property for O(log n) performance
- In-order traversal can verify: count nodes in subtree and compare to stored size

---

## Implementation Notes

1. **NULL handling:** Helper function `SIZE(x)` returns 0 for NULL nodes, 1 + SIZE(left) + SIZE(right) otherwise

2. **Initialization:** New nodes created with `size = 1`

3. **No rotation needed:** Unlike Red-Black tree implementations, our BST-based approach doesn't need to update sizes during rotations

4. **Testing:** Verified size maintenance in `os_main.c` test program:
   - After insertions: all sizes correct
   - After deletions: all sizes correct
   - OS-SELECT and OS-RANK return correct results

---

## Complexity Analysis

Both operations maintain O(h) complexity where h is tree height:

- **OS-TREE-INSERT:** O(h) traversal + O(1) per node size update = O(h)
- **OS-TREE-DELETE:** O(h) traversal + O(h) worst case for successor path = O(h)

For balanced BSTs: h = O(log n), giving O(log n) insert and delete with size maintenance.

---

## References

- CLRS 3rd Edition, Chapter 14.1: Dynamic order statistics
- Assignment implementation: `src/os_tree.c` lines 31-121
- Test verification: `src/os_main.c`
