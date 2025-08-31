# Sorting Algorithms Program

## Overview
Interactive C program implementing multiple sorting algorithms with visualization and performance testing capabilities.
## Compilation & Usage
```bash
gcc -o sorting_algorithms sorting_algorithms.c
./sorting_algorithms
```

## Data Structures

### SortStats
```c
typedef struct {
    unsigned long long comparisons;  // Counts element comparisons
    unsigned long long swaps;        // Counts element swaps
    double execution_time_ms;        // Execution time in milliseconds
} SortStats;
```

### Node (Linked List)
```c
typedef struct Node {
    char info[MAX_STRING_LENGTH];    // String data (max 100 chars)
    struct Node* next;               // Pointer to next node
} Node;
```

## Helper Functions

### Time & Display
- `clearScreen()` - Clears console (Windows/Linux compatible)
- `getTimeMs()` - Returns current time in milliseconds with microsecond precision
- `printSeparator()` - Prints visual separator (60 '=' characters)
- `printHeader(title)` - Prints formatted section headers

### Data Display
- `printArray(arr, size)` - Displays array with smart truncation (max 20 elements)
- `printMatrix(matrix, rows)` - Displays 2D string matrix
- `printList(head)` - Displays linked list contents
- `printStats(stats, showTime)` - Shows comparisons, swaps, execution time

### Data Generation
- `generateRandomArray(size)` - Creates random integer array
- `getUserArray(size)` - Prompts user for array values
- `generateRandomMatrix(rows)` - Creates random string matrix
- `getUserMatrix(rows)` - Prompts user for matrix strings
- `generateRandomList(size)` - Creates random linked list
- `getUserList(size)` - Prompts user for list strings

### Memory Management
- `freeList(head)` - Deallocates linked list memory
- `freeMatrix(matrix, rows)` - Deallocates matrix memory

## Sorting Algorithms

### Array Sorting
1. **Bubble Sort**
   - **Average**: O(n²)
   - **Worst**: O(n²)
   - **Best**: O(n)
   - **Space**: O(1)
   - **Function**: `bubbleSortArray()`

2. **Selection Sort**
   - **Average**: O(n²)
   - **Worst**: O(n²)
   - **Best**: O(n²)
   - **Space**: O(1)
   - **Function**: `selectionSortArray()`

3. **Insertion Sort**
   - **Average**: O(n²)
   - **Worst**: O(n²)
   - **Best**: O(n)
   - **Space**: O(1)
   - **Function**: `insertionSortArray()`

4. **Merge Sort**
   - **Average**: O(n log n)
   - **Worst**: O(n log n)
   - **Best**: O(n log n)
   - **Space**: O(n)
   - **Function**: `mergeSortArray()`

5. **Quick Sort**
   - **Average**: O(n log n)
   - **Worst**: O(n²)
   - **Best**: O(n log n)
   - **Space**: O(log n)
   - **Function**: `quickSortArray()`

6. **Comb Sort**
   - **Average**: O(n²)
   - **Worst**: O(n²)
   - **Best**: O(n log n)
   - **Space**: O(1)
   - **Function**: `combSortArray()`

### Matrix Sorting
- **Bubble Sort for Strings**
  - **Function**: `bubbleSortMatrix()`
  - **Uses**: `strcmp()` for comparison, `swapStrings()` for swapping

### Linked List Sorting
1. **Bubble Sort**
   - **Function**: `bubbleSortList()`
   - **Modifies**: List structure by swapping nodes

2. **Insertion Sort**
   - **Function**: `insertionSortList()`
   - **Modifies**: List structure by inserting nodes

## Program Structure

### Main Menu System
```
main()
├── printMainMenu()
├── arraySorting()
│   ├── printAlgorithmMenu("Array")
│   ├── printOrderMenu()
│   ├── printVisualizationMenu()
│   └── [Sorting Algorithm Functions]
├── matrixSorting()
│   ├── printAlgorithmMenu("Matrix")
│   ├── printOrderMenu()
│   ├── printVisualizationMenu()
│   └── bubbleSortMatrix()
├── listSorting()
│   ├── printAlgorithmMenu("List")
│   ├── printOrderMenu()
│   ├── printVisualizationMenu()
│   └── [List Sorting Functions]
└── performanceTesting()
    ├── [Array Performance Tests]
    └── [Statistics Display]
```

### Key Variables
- `MAX_STRING_LENGTH = 100` - Maximum string length for matrix/list elements
- `MAX_DISPLAY_SIZE = 20` - Maximum elements to display for large datasets
- `SEPARATOR_LENGTH = 60` - Length of visual separators
- `ascending` - Boolean for sort order (true=ascending, false=descending)
- `showSteps` - Boolean for step-by-step visualization
- `size` - Data structure size (unsigned int, no limits)

### Performance Testing
- **Mode**: Separate from visualization
- **Features**: 
  - No step-by-step printing
  - High-precision timing (microseconds → milliseconds)
  - Overflow-safe counters (unsigned long long)
  - Individual or all algorithms testing
  - Complete statistics display


