

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

// ============================================================================
// CONSTANTS AND CONFIGURATION
// ============================================================================

#define MAX_STRING_LENGTH 100
#define MAX_DISPLAY_SIZE 50  // Limit display for large arrays
#define SEPARATOR_LENGTH 60

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    unsigned long long comparisons;  // Prevent overflow for large datasets
    unsigned long long swaps;        // Prevent overflow for large datasets
    double execution_time_ms;        // Time in milliseconds
} SortStats;

typedef struct Node {
    char info[MAX_STRING_LENGTH];
    struct Node* next;
} Node;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

double getTimeMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void printSeparator() {
    printf("\n");
    for (int i = 0; i < SEPARATOR_LENGTH; i++) printf("=");
    printf("\n");
}

void printHeader(const char* title) {
    printSeparator();
    printf("%s\n", title);
    printSeparator();
}

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

void printArray(int* arr, unsigned int size) {
    printf("Array: [");
    unsigned int displaySize = (size > MAX_DISPLAY_SIZE) ? MAX_DISPLAY_SIZE : size;
    
    for (unsigned int i = 0; i < displaySize; i++) {
        printf("%d", arr[i]);
        if (i < displaySize - 1) printf(", ");
    }
    
    if (size > MAX_DISPLAY_SIZE) {
        printf(" ... (%u more elements)", size - MAX_DISPLAY_SIZE);
    }
    printf("]\n");
}

void printMatrix(char** matrix, unsigned int rows) {
    printf("Matrix:\n");
    for (unsigned int i = 0; i < rows; i++) {
        printf("  [%u]: %s\n", i, matrix[i]);
    }
}

void printList(Node* head) {
    printf("List: ");
    unsigned int count = 0;
    while (head != NULL && count < MAX_DISPLAY_SIZE) {
        printf("%s", head->info);
        if (head->next != NULL) printf(" -> ");
        head = head->next;
        count++;
    }
    if (head != NULL) {
        printf(" ... (more elements)");
    }
    printf(" -> NULL\n");
}

void printStats(const SortStats* stats, bool showTime) {
    printf("Comparisons: %llu\n", stats->comparisons);
    printf("Swaps: %llu\n", stats->swaps);
    if (showTime) {
        printf("Execution time: %.6f milliseconds\n", stats->execution_time_ms);
    }
}

// ============================================================================
// SWAP FUNCTIONS
// ============================================================================

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapStrings(char* a, char* b) {
    char temp[MAX_STRING_LENGTH];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

// ============================================================================
// DATA GENERATION FUNCTIONS
// ============================================================================

int* generateRandomArray(unsigned int size) {
    int* arr = malloc(size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    srand(time(NULL));
    for (unsigned int i = 0; i < size; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

int* getUserArray(unsigned int size) {
    int* arr = malloc(size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    printf("Enter %u integers:\n", size);
    for (unsigned int i = 0; i < size; i++) {
        printf("Enter value %u: ", i + 1);
        scanf("%d", &arr[i]);
    }
    return arr;
}

char** generateRandomMatrix(unsigned int rows) {
    char** matrix = malloc(rows * sizeof(char*));
    if (!matrix) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    srand(time(NULL));
    for (unsigned int i = 0; i < rows; i++) {
        matrix[i] = malloc(MAX_STRING_LENGTH * sizeof(char));
        if (!matrix[i]) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        sprintf(matrix[i], "word%u", rand() % 10000);
    }
    return matrix;
}

char** getUserMatrix(unsigned int rows) {
    char** matrix = malloc(rows * sizeof(char*));
    if (!matrix) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    printf("Enter %u strings (max %d characters each):\n", rows, MAX_STRING_LENGTH - 1);
    for (unsigned int i = 0; i < rows; i++) {
        matrix[i] = malloc(MAX_STRING_LENGTH * sizeof(char));
        if (!matrix[i]) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        printf("Enter string %u: ", i + 1);
        scanf("%s", matrix[i]);
    }
    return matrix;
}

Node* createNode(const char* info) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(newNode->info, info);
    newNode->next = NULL;
    return newNode;
}

Node* generateRandomList(unsigned int size) {
    Node* head = NULL;
    Node* tail = NULL;
    srand(time(NULL));
    
    for (unsigned int i = 0; i < size; i++) {
        char word[MAX_STRING_LENGTH];
        sprintf(word, "word%u", rand() % 10000);
        Node* newNode = createNode(word);
        
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

Node* getUserList(unsigned int size) {
    Node* head = NULL;
    Node* tail = NULL;
    
    printf("Enter %u strings:\n", size);
    for (unsigned int i = 0; i < size; i++) {
        char word[MAX_STRING_LENGTH];
        printf("Enter string %u: ", i + 1);
        scanf("%s", word);
        Node* newNode = createNode(word);
        
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// ============================================================================
// MEMORY MANAGEMENT
// ============================================================================

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeMatrix(char** matrix, unsigned int rows) {
    for (unsigned int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// ============================================================================
// SORTING ALGORITHMS - ARRAYS
// ============================================================================

void bubbleSortArray(int* arr, unsigned int size, bool ascending, SortStats* stats, bool showSteps) {
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("BUBBLE SORT");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printArray(arr, size);
    }
    
    bool swapped;
    for (unsigned int i = 0; i < size - 1; i++) {
        swapped = false;
        for (unsigned int j = 0; j < size - i - 1; j++) {
            stats->comparisons++;
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                swap(&arr[j], &arr[j + 1]);
                stats->swaps++;
                swapped = true;
                if (showSteps) {
                    printf("Step %u (Swap %llu): ", i + 1, stats->swaps);
                    printArray(arr, size);
                }
            }
        }
        if (!swapped && showSteps) {
            printf("No more swaps needed. Array is sorted!\n");
            break;
        }
    }
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

void selectionSortArray(int* arr, unsigned int size, bool ascending, SortStats* stats, bool showSteps) {
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("SELECTION SORT");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printArray(arr, size);
    }
    
    for (unsigned int i = 0; i < size - 1; i++) {
        unsigned int extreme = i;
        
        for (unsigned int j = i + 1; j < size; j++) {
            stats->comparisons++;
            if ((ascending && arr[j] < arr[extreme]) || (!ascending && arr[j] > arr[extreme])) {
                extreme = j;
            }
        }
        
        if (extreme != i) {
            swap(&arr[extreme], &arr[i]);
            stats->swaps++;
            if (showSteps) {
                printf("Step %u (Swap %llu): ", i + 1, stats->swaps);
                printArray(arr, size);
            }
        }
    }
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

void insertionSortArray(int* arr, unsigned int size, bool ascending, SortStats* stats, bool showSteps) {
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("INSERTION SORT");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printArray(arr, size);
    }
    
    for (unsigned int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && ((ascending && arr[j] > key) || (!ascending && arr[j] < key))) {
            arr[j + 1] = arr[j];
            j--;
            stats->comparisons++;
            stats->swaps++;
        }
        stats->comparisons++;
        arr[j + 1] = key;
        if (showSteps) {
            printf("Step %u (Insert %d): ", i, key);
            printArray(arr, size);
        }
    }
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

void merge(int* arr, int left, int mid, int right, bool ascending, SortStats* stats, bool showSteps) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int* L = malloc(n1 * sizeof(int));
    int* R = malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        stats->comparisons++;
        if ((ascending && L[i] <= R[j]) || (!ascending && L[i] >= R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
            stats->swaps++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++; k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
    }
    
    if (showSteps) {
        printf("Merge step [%d-%d]: ", left, right);
        printArray(arr, right + 1);
    }
    
    free(L); free(R);
}

void mergeSortArray(int* arr, int left, int right, bool ascending, SortStats* stats, bool showSteps) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortArray(arr, left, mid, ascending, stats, showSteps);
        mergeSortArray(arr, mid + 1, right, ascending, stats, showSteps);
        merge(arr, left, mid, right, ascending, stats, showSteps);
    }
}

void quickSortArray(int* arr, int low, int high, bool ascending, SortStats* stats, bool showSteps) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            stats->comparisons++;
            if ((ascending && arr[j] < pivot) || (!ascending && arr[j] > pivot)) {
                i++;
                swap(&arr[i], &arr[j]);
                stats->swaps++;
            }
        }
        swap(&arr[i + 1], &arr[high]);
        stats->swaps++;
        
        if (showSteps) {
            printf("Pivot step: ");
            printArray(arr, high + 1);
        }
        
        quickSortArray(arr, low, i, ascending, stats, showSteps);
        quickSortArray(arr, i + 2, high, ascending, stats, showSteps);
    }
}

void combSortArray(int* arr, unsigned int size, bool ascending, SortStats* stats, bool showSteps) {
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("COMB SORT");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printArray(arr, size);
    }
    
    int gap = size;
    bool swapped = true;
    
    while (gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;
        swapped = false;
        
        for (unsigned int i = 0; i + gap < size; i++) {
            int j = i + gap;
            stats->comparisons++;
            if ((ascending && arr[i] > arr[j]) || (!ascending && arr[i] < arr[j])) {
                swap(&arr[i], &arr[j]);
                stats->swaps++;
                swapped = true;
                if (showSteps) {
                    printf("Step (gap=%d): ", gap);
                    printArray(arr, size);
                }
            }
        }
    }
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

// ============================================================================
// SORTING ALGORITHMS - MATRICES
// ============================================================================

void bubbleSortMatrix(char** matrix, unsigned int rows, bool ascending, SortStats* stats, bool showSteps) {
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("BUBBLE SORT - MATRIX");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printMatrix(matrix, rows);
    }
    
    bool swapped;
    do {
        swapped = false;
        for (unsigned int i = 0; i < rows - 1; i++) {
            stats->comparisons++;
            if ((ascending && strcmp(matrix[i], matrix[i + 1]) > 0) ||
                (!ascending && strcmp(matrix[i], matrix[i + 1]) < 0)) {
                swapStrings(matrix[i], matrix[i + 1]);
                stats->swaps++;
                swapped = true;
                if (showSteps) {
                    printf("Step %llu (Swap):\n", stats->swaps);
                    printMatrix(matrix, rows);
                }
            }
        }
    } while (swapped);
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

// ============================================================================
// SORTING ALGORITHMS - LINKED LISTS
// ============================================================================

void bubbleSortList(Node** head, bool ascending, SortStats* stats, bool showSteps) {
    if (*head == NULL) return;
    
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("BUBBLE SORT - LINKED LIST");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printList(*head);
    }
    
    bool swapped;
    Node** current;
    
    do {
        swapped = false;
        current = head;
        
        while ((*current)->next != NULL) {
            Node* a = *current;
            Node* b = a->next;
            
            stats->comparisons++;
            if ((ascending && strcmp(a->info, b->info) > 0) ||
                (!ascending && strcmp(a->info, b->info) < 0)) {
                a->next = b->next;
                b->next = a;
                *current = b;
                stats->swaps++;
                swapped = true;
                if (showSteps) {
                    printf("Step %llu (Swap): ", stats->swaps);
                    printList(*head);
                }
            }
            current = &(*current)->next;
        }
    } while (swapped);
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

void insertionSortList(Node** head, bool ascending, SortStats* stats, bool showSteps) {
    if (*head == NULL) return;
    
    stats->comparisons = 0;
    stats->swaps = 0;
    double startTime = getTimeMs();
    
    if (showSteps) {
        printHeader("INSERTION SORT - LINKED LIST");
        printf("Order: %s\n", ascending ? "Ascending" : "Descending");
        printList(*head);
    }
    
    Node* sorted = NULL;
    Node* current = *head;
    
    while (current != NULL) {
        Node* next = current->next;
        
        if (sorted == NULL ||
            (ascending && strcmp(current->info, sorted->info) < 0) ||
            (!ascending && strcmp(current->info, sorted->info) > 0)) {
            current->next = sorted;
            sorted = current;
            stats->swaps++;
            stats->comparisons++;
        } else {
            Node* temp = sorted;
            while (temp->next != NULL &&
                   ((ascending && strcmp(current->info, temp->next->info) > 0) ||
                    (!ascending && strcmp(current->info, temp->next->info) < 0))) {
                temp = temp->next;
                stats->comparisons++;
            }
            current->next = temp->next;
            temp->next = current;
            stats->swaps++;
        }
        current = next;
        if (showSteps) {
            printf("Step %llu: ", stats->swaps);
            printList(sorted);
        }
    }
    *head = sorted;
    
    stats->execution_time_ms = getTimeMs() - startTime;
}

// ============================================================================
// USER INTERFACE FUNCTIONS
// ============================================================================

void printMainMenu() {
    clearScreen();
    printf("\n=== INTERACTIVE SORTING ALGORITHMS ===\n");
    printf("1. Array/Vector Sorting\n");
    printf("2. Matrix/String Array Sorting\n");
    printf("3. Linked List Sorting\n");
    printf("4. Performance Testing Mode\n");
    printf("5. Exit\n");
    printf("Choose option (1-5): ");
}

void printAlgorithmMenu(const char* dataType) {
    printf("\nChoose sorting algorithm for %s:\n", dataType);
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("6. Comb Sort\n");
    printf("Choice: ");
}

void printOrderMenu() {
    printf("Choose sort order:\n");
    printf("1. Ascending\n");
    printf("2. Descending\n");
    printf("Choice: ");
}

void printVisualizationMenu() {
    printf("Show step-by-step visualization?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Choice: ");
}

// ============================================================================
// MAIN SORTING FUNCTIONS
// ============================================================================

void arraySorting() {
    clearScreen();
    printHeader("ARRAY SORTING");
    
    // Get array size
    unsigned int size;
    printf("Enter array size: ");
    scanf("%u", &size);
    if (size == 0) {
        printf("Invalid size!\n");
        return;
    }
    
    // Get input method
    int choice;
    printf("Choose data input method:\n");
    printf("1. Random data\n");
    printf("2. User input\n");
    printf("Choice: ");
    scanf("%d", &choice);
    
    // Generate or get array
    int* arr;
    if (choice == 1) {
        arr = generateRandomArray(size);
        printf("Generated random array:\n");
    } else {
        arr = getUserArray(size);
        printf("Your array:\n");
    }
    printArray(arr, size);
    
    // Get algorithm choice
    printAlgorithmMenu("Array");
    scanf("%d", &choice);
    
    // Get sort order
    printOrderMenu();
    int order;
    scanf("%d", &order);
    bool ascending = (order == 1);
    
    // Get visualization preference
    printVisualizationMenu();
    int viz;
    scanf("%d", &viz);
    bool showSteps = (viz == 1);
    
    // Sort the array
    SortStats stats;
    int* arrCopy = malloc(size * sizeof(int));
    memcpy(arrCopy, arr, size * sizeof(int));
    
    switch (choice) {
        case 1:
            bubbleSortArray(arrCopy, size, ascending, &stats, showSteps);
            break;
        case 2:
            selectionSortArray(arrCopy, size, ascending, &stats, showSteps);
            break;
        case 3:
            insertionSortArray(arrCopy, size, ascending, &stats, showSteps);
            break;
        case 4:
            stats.comparisons = 0;
            stats.swaps = 0;
            double startTime = getTimeMs();
            if (showSteps) {
                printHeader("MERGE SORT");
                printf("Order: %s\n", ascending ? "Ascending" : "Descending");
                printArray(arrCopy, size);
            }
            mergeSortArray(arrCopy, 0, size - 1, ascending, &stats, showSteps);
            stats.execution_time_ms = getTimeMs() - startTime;
            break;
        case 5:
            stats.comparisons = 0;
            stats.swaps = 0;
            startTime = getTimeMs();
            if (showSteps) {
                printHeader("QUICK SORT");
                printf("Order: %s\n", ascending ? "Ascending" : "Descending");
                printArray(arrCopy, size);
            }
            quickSortArray(arrCopy, 0, size - 1, ascending, &stats, showSteps);
            stats.execution_time_ms = getTimeMs() - startTime;
            break;
        case 6:
            combSortArray(arrCopy, size, ascending, &stats, showSteps);
            break;
        default:
            printf("Invalid choice!\n");
            free(arrCopy);
            free(arr);
            return;
    }
    
    // Display results
    printHeader("SORTING COMPLETED");
    printStats(&stats, !showSteps);
    printf("Final sorted array: ");
    printArray(arrCopy, size);
    
    free(arrCopy);
    free(arr);
}

void matrixSorting() {
    clearScreen();
    printHeader("MATRIX SORTING");
    
    // Get matrix size
    unsigned int rows;
    printf("Enter number of strings: ");
    scanf("%u", &rows);
    if (rows == 0) {
        printf("Invalid number!\n");
        return;
    }
    
    // Get input method
    int choice;
    printf("Choose data input method:\n");
    printf("1. Random data\n");
    printf("2. User input\n");
    printf("Choice: ");
    scanf("%d", &choice);
    
    // Generate or get matrix
    char** matrix;
    if (choice == 1) {
        matrix = generateRandomMatrix(rows);
        printf("Generated random matrix:\n");
    } else {
        matrix = getUserMatrix(rows);
        printf("Your matrix:\n");
    }
    printMatrix(matrix, rows);
    
    // Get sort order
    printOrderMenu();
    int order;
    scanf("%d", &order);
    bool ascending = (order == 1);
    
    // Get visualization preference
    printVisualizationMenu();
    int viz;
    scanf("%d", &viz);
    bool showSteps = (viz == 1);
    
    // Sort the matrix
    SortStats stats;
    bubbleSortMatrix(matrix, rows, ascending, &stats, showSteps);
    
    // Display results
    printHeader("SORTING COMPLETED");
    printStats(&stats, !showSteps);
    printf("Final sorted matrix:\n");
    printMatrix(matrix, rows);
    
    freeMatrix(matrix, rows);
}

void listSorting() {
    clearScreen();
    printHeader("LINKED LIST SORTING");
    
    // Get list size
    unsigned int size;
    printf("Enter number of strings: ");
    scanf("%u", &size);
    if (size == 0) {
        printf("Invalid number!\n");
        return;
    }
    
    // Get input method
    int choice;
    printf("Choose data input method:\n");
    printf("1. Random data\n");
    printf("2. User input\n");
    printf("Choice: ");
    scanf("%d", &choice);
    
    // Generate or get list
    Node* head;
    if (choice == 1) {
        head = generateRandomList(size);
        printf("Generated random list:\n");
    } else {
        head = getUserList(size);
        printf("Your list:\n");
    }
    printList(head);
    
    // Get algorithm choice
    printf("\nChoose sorting algorithm for Linked List:\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("Choice: ");
    scanf("%d", &choice);
    
    // Get sort order
    printOrderMenu();
    int order;
    scanf("%d", &order);
    bool ascending = (order == 1);
    
    // Get visualization preference
    printVisualizationMenu();
    int viz;
    scanf("%d", &viz);
    bool showSteps = (viz == 1);
    
    // Sort the list
    SortStats stats;
    switch (choice) {
        case 1:
            bubbleSortList(&head, ascending, &stats, showSteps);
            break;
        case 2:
            insertionSortList(&head, ascending, &stats, showSteps);
            break;
        default:
            printf("Invalid choice!\n");
            freeList(head);
            return;
    }
    
    // Display results
    printHeader("SORTING COMPLETED");
    printStats(&stats, !showSteps);
    printf("Final sorted list: ");
    printList(head);
    
    freeList(head);
}

void performanceTesting() {
    clearScreen();
    printHeader("PERFORMANCE TESTING MODE");
    printf("(No visualization - pure performance measurement)\n\n");
    
    printf("Performance testing is available for Array/Vector sorting only.\n");
    printf("Matrix and Linked List performance testing will be added in future versions.\n\n");
    
    // Get test parameters
    unsigned int size;
    printf("Enter array size for performance testing: ");
    scanf("%u", &size);
    if (size == 0) {
        printf("Invalid size!\n");
        return;
    }
    
    printOrderMenu();
    int order;
    scanf("%d", &order);
    bool ascending = (order == 1);
    
    printf("Choose testing mode:\n");
    printf("1. Test specific algorithm\n");
    printf("2. Test all algorithms\n");
    printf("Choice: ");
    int testChoice;
    scanf("%d", &testChoice);
    
    printf("Generating random array of size %u...\n", size);
    int* arr = generateRandomArray(size);
    int* arrCopy = malloc(size * sizeof(int));
    SortStats stats;
    
    if (testChoice == 1) {
        // Test specific algorithm
        printAlgorithmMenu("Array");
        int algChoice;
        scanf("%d", &algChoice);
        
        printf("\nTesting Array/Vector sorting algorithm...\n");
        printf("Size: %u, Order: %s\n\n", size, ascending ? "Ascending" : "Descending");
        printf("%-15s %-15s %-20s %-20s\n", "Algorithm", "Time (ms)", "Comparisons", "Swaps");
        printf("----------------------------------------------------------------\n");
        
        switch (algChoice) {
            case 1:
                memcpy(arrCopy, arr, size * sizeof(int));
                bubbleSortArray(arrCopy, size, ascending, &stats, false);
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Bubble Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            case 2:
                memcpy(arrCopy, arr, size * sizeof(int));
                selectionSortArray(arrCopy, size, ascending, &stats, false);
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Selection Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            case 3:
                memcpy(arrCopy, arr, size * sizeof(int));
                insertionSortArray(arrCopy, size, ascending, &stats, false);
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Insertion Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            case 4:
                memcpy(arrCopy, arr, size * sizeof(int));
                stats.comparisons = 0;
                stats.swaps = 0;
                double startTime = getTimeMs();
                mergeSortArray(arrCopy, 0, size - 1, ascending, &stats, false);
                stats.execution_time_ms = getTimeMs() - startTime;
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Merge Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            case 5:
                memcpy(arrCopy, arr, size * sizeof(int));
                stats.comparisons = 0;
                stats.swaps = 0;
                startTime = getTimeMs();
                quickSortArray(arrCopy, 0, size - 1, ascending, &stats, false);
                stats.execution_time_ms = getTimeMs() - startTime;
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Quick Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            case 6:
                memcpy(arrCopy, arr, size * sizeof(int));
                combSortArray(arrCopy, size, ascending, &stats, false);
                printf("%-15s %-15.6f %-20llu %-20llu\n", "Comb Sort", stats.execution_time_ms, stats.comparisons, stats.swaps);
                break;
            default:
                printf("Invalid choice!\n");
                free(arrCopy);
                free(arr);
                return;
        }
    } else {
        // Test all algorithms
        printf("\nTesting all Array/Vector sorting algorithms...\n");
        printf("Size: %u, Order: %s\n\n", size, ascending ? "Ascending" : "Descending");
        printf("%-15s %-15s %-20s %-20s\n", "Algorithm", "Time (ms)", "Comparisons", "Swaps");
        printf("----------------------------------------------------------------\n");
        
        // Test each algorithm
        const char* algorithms[] = {"Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort", "Quick Sort", "Comb Sort"};
        
        for (int i = 0; i < 6; i++) {
            memcpy(arrCopy, arr, size * sizeof(int));
            
            switch (i) {
                case 0:
                    bubbleSortArray(arrCopy, size, ascending, &stats, false);
                    break;
                case 1:
                    selectionSortArray(arrCopy, size, ascending, &stats, false);
                    break;
                case 2:
                    insertionSortArray(arrCopy, size, ascending, &stats, false);
                    break;
                case 3:
                    stats.comparisons = 0;
                    stats.swaps = 0;
                    double startTime = getTimeMs();
                    mergeSortArray(arrCopy, 0, size - 1, ascending, &stats, false);
                    stats.execution_time_ms = getTimeMs() - startTime;
                    break;
                case 4:
                    stats.comparisons = 0;
                    stats.swaps = 0;
                    startTime = getTimeMs();
                    quickSortArray(arrCopy, 0, size - 1, ascending, &stats, false);
                    stats.execution_time_ms = getTimeMs() - startTime;
                    break;
                case 5:
                    combSortArray(arrCopy, size, ascending, &stats, false);
                    break;
            }
            
            printf("%-15s %-15.6f %-20llu %-20llu\n", algorithms[i], stats.execution_time_ms, stats.comparisons, stats.swaps);
        }
    }
    
    free(arrCopy);
    free(arr);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    int choice;
    
    do {
        printMainMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                arraySorting();
                break;
            case 2:
                matrixSorting();
                break;
            case 3:
                listSorting();
                break;
            case 4:
                performanceTesting();
                break;
            case 5:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        if (choice != 5) {
            printf("\nPress Enter to continue...");
            getchar();
            getchar();
        }
    } while (choice != 5);
    
    return 0;
}
