#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PRODUCTS 100
#define MAX_HISTORY_SIZE 100
#define MAX_STRING_LENGTH 100

// Enumeration for user roles
typedef enum {
    ADMIN,
    EMPLOYEE,
    UNKNOWN
} UserRole;

// Structure to represent a product
typedef struct {
    int id;
    char name[MAX_STRING_LENGTH];
    float price;
    int quantity;
} Product;

// Structure to represent a transaction
typedef struct {
    char type[MAX_STRING_LENGTH];  
    Product product;                
    int quantity;                   
    time_t timestamp;               
} Transaction;

// Structure to represent user credentials
typedef struct {
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    UserRole role;
} User;

// Function prototypes
void displayMenu(UserRole role);
void loginUser(User *users, int numUsers, User *loggedInUser);
bool authenticateUser(User *users, int numUsers, const char *username, const char *password);
void addProduct(Product *inventory, int *size, Transaction *history, int *historySize, UserRole role);
void deleteProduct(Product *inventory, int *size, Transaction *history, int *historySize, UserRole role);
void updateProduct(Product *inventory, int size, Transaction *history, int *historySize, UserRole role);
void displayInventory(Product *inventory, int size);
void sortInventoryById(Product *inventory, int size);
void sortInventoryByName(Product *inventory, int size);
void sortInventoryByPrice(Product *inventory, int size);
void searchProduct(Product *inventory, int size);
void generateReport(Product *inventory, int size);
void handleSales(Product *inventory, int size);
void displayTransactionHistory(Transaction *history, int historySize);

int main() {
    
    Product inventory[MAX_PRODUCTS];
    int size = 0;

    Transaction history[MAX_HISTORY_SIZE];
    int historySize = 0;

    User users[] = {
        {"admin", "admin123", ADMIN},
        {"employee", "emp456", EMPLOYEE}
    };
    int numUsers = sizeof(users) / sizeof(users[0]);

    User loggedInUser;

    // Login user
    loginUser(users, numUsers, &loggedInUser);

    if (loggedInUser.role == UNKNOWN) {
        printf("Invalid username or password. Exiting...\n");
        return 1;
    }

    int choice;
    do {
        displayMenu(loggedInUser.role);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct(inventory, &size, history, &historySize, loggedInUser.role);
                break;
            case 2:
                deleteProduct(inventory, &size, history, &historySize, loggedInUser.role);
                break;
            case 3:
                updateProduct(inventory, size, history, &historySize, loggedInUser.role);
                break;
            case 4:
                displayInventory(inventory, size);
                break;
            case 5:
                sortInventoryById(inventory, size);
                break;
            case 6:
                sortInventoryByName(inventory, size);
                break;
            case 7:
                sortInventoryByPrice(inventory, size);
                break;
            case 8:
                searchProduct(inventory, size);
                break;
            case 9:
                generateReport(inventory, size);
                break;
            case 10:
                handleSales(inventory, size);
                break;
            case 11:
                displayTransactionHistory(history, historySize);
                break;
            case 12:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}

void displayMenu(UserRole role) {
    printf("\n=== Inventory Management System ===\n");
    printf("1. Add Product\n");
    printf("2. Delete Product\n");
    printf("3. Update Product\n");
    printf("4. Display Inventory\n");
    printf("5. Sort Inventory by ID\n");
    printf("6. Sort Inventory by Name\n");
    printf("7. Sort Inventory by Price\n");
    printf("8. Search Product\n");
    printf("9. Generate Report\n");
    printf("10. Handle Sales\n");
    printf("11. Display Transaction History\n");
    printf("12. Logout\n");

    if (role == ADMIN) {
        printf("13. Admin Option 1\n");
        printf("14. Admin Option 2\n");
    } else if (role == EMPLOYEE) {
        printf("13. Employee Option 1\n");
        printf("14. Employee Option 2\n");
    }
}


void loginUser(User *users, int numUsers, User *loggedInUser) {
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (authenticateUser(users, numUsers, username, password)) {
        printf("Login successful!\n");
        for (int i = 0; i < numUsers; i++) {
            if (strcmp(users[i].username, username) == 0) {
                *loggedInUser = users[i];
                break;
            }
        }
    } else {
        printf("Invalid username or password.\n");
        loggedInUser->role = UNKNOWN;
    }
}

// Function to authenticate user
bool authenticateUser(User *users, int numUsers, const char *username, const char *password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return true;
        }
    }
    return false;
}

// Function to add a product to inventory
void addProduct(Product *inventory, int *size, Transaction *history, int *historySize, UserRole role) {
    if (role != ADMIN) {
        printf("Access denied! You do not have permission to add products.\n");
        return;
    }

    if (*size == MAX_PRODUCTS) {
        printf("Inventory is full. Cannot add more products.\n");
        return;
    }

    Product newProduct;

    printf("Enter product ID: ");
    scanf("%d", &newProduct.id);
    printf("Enter product name: ");
    scanf("%s", newProduct.name);
    printf("Enter product price: ");
    scanf("%f", &newProduct.price);
    printf("Enter product quantity: ");
    scanf("%d", &newProduct.quantity);

    inventory[*size] = newProduct;

    // Log transaction
    history[*historySize].timestamp = time(NULL);
    strcpy(history[*historySize].type, "Add");
    history[*historySize].product = newProduct;
    history[*historySize].quantity = newProduct.quantity;
    (*historySize)++;

    (*size)++;
    printf("Product added successfully.\n");
}

// Function to delete a product from inventory
void deleteProduct(Product *inventory, int *size, Transaction *history, int *historySize, UserRole role) {
    if (role != ADMIN) {
        printf("Access denied! You do not have permission to delete products.\n");
        return;
    }

    int id, found = 0;

    printf("Enter product ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < *size; i++) {
        if(inventory[i].id == id) {
            found = 1;
            // Log transaction
            history[*historySize].timestamp = time(NULL);
            strcpy(history[*historySize].type, "Delete");
            history[*historySize].product = inventory[i];
            history[*historySize].quantity = inventory[i].quantity;
            (*historySize)++;

            for(int j = i; j < *size - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            (*size)--;
            printf("Product deleted successfully.\n");
            break;
        }
    }

    if(!found) {
        printf("Product with ID %d not found.\n", id);
    }
}

// Function to update a product in inventory
void updateProduct(Product *inventory, int size, Transaction *history, int *historySize, UserRole role) {
    if (role != ADMIN) {
        printf("Access denied! You do not have permission to update products.\n");
        return;
    }

    int id, found = 0;

    printf("Enter product ID to update: ");
    scanf("%d", &id);

    for(int i = 0; i < size; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf("Enter new name: ");
            scanf("%s", inventory[i].name);
            printf("Enter new price: ");
            scanf("%f", &inventory[i].price);
            printf("Enter new quantity: ");
            scanf("%d", &inventory[i].quantity);

            // Log transaction
            history[*historySize].timestamp = time(NULL);
            strcpy(history[*historySize].type, "Update");
            history[*historySize].product = inventory[i];
            history[*historySize].quantity = inventory[i].quantity;
            (*historySize)++;

            printf("Product updated successfully.\n");
            break;
        }
    }

    if(!found) {
        printf("Product with ID %d not found.\n", id);
    }
}

// Function to display inventory
void displayInventory(Product *inventory, int size) {
    printf("\n=== Inventory ===\n");
    for(int i = 0; i < size; i++) {
        printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
}

// Function to sort inventory based on ID
void sortInventoryById(Product *inventory, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (inventory[j].id > inventory[j + 1].id) {
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
    printf("Inventory sorted by ID successfully.\n");
}

// Function to sort inventory based on Name
void sortInventoryByName(Product *inventory, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (strcmp(inventory[j].name, inventory[j + 1].name) > 0) {
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
    printf("Inventory sorted by Name successfully.\n");
}

// Function to sort inventory based on Price
void sortInventoryByPrice(Product *inventory, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (inventory[j].price > inventory[j + 1].price) {
                Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
    printf("Inventory sorted by Price successfully.\n");
}

// Function to search for a product by name
void searchProduct(Product *inventory, int size) {
    char name[MAX_STRING_LENGTH];
    int found = 0;

    printf("Enter product name to search: ");
    scanf("%s", name);

    for(int i = 0; i < size; i++) {
        if(strcmp(inventory[i].name, name) == 0) {
            found = 1;
            printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
        }
    }

    if(!found) {
        printf("Product with name '%s' not found.\n", name);
    }
}

// Function to generate inventory status report
void generateReport(Product *inventory, int size) {
    float totalValue = 0;
    int totalProducts = 0;
    Product highestSellingProduct = inventory[0];

    for (int i = 0; i < size; i++) {
        totalValue += inventory[i].price * inventory[i].quantity;
        totalProducts += inventory[i].quantity;
        if (inventory[i].quantity > highestSellingProduct.quantity) {
            highestSellingProduct = inventory[i];
        }
    }

    printf("\n=== Inventory Report ===\n");
    printf("Total number of products: %d\n", size);
    printf("Total inventory value: %.2f\n", totalValue);
    printf("Highest selling product: ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
           highestSellingProduct.id, highestSellingProduct.name, highestSellingProduct.price, highestSellingProduct.quantity);
    printf("Total quantity of products: %d\n", totalProducts);
}

// Function to handle product sales
void handleSales(Product *inventory, int size) {
    int id, quantity, found = 0;

    printf("Enter product ID for sale: ");
    scanf("%d", &id);

    for(int i = 0; i < size; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf("Enter quantity for sale: ");
            scanf("%d", &quantity);
            if(inventory[i].quantity >= quantity) {
                inventory[i].quantity -= quantity;
                printf("Sale successful. %d units of %s sold.\n", quantity, inventory[i].name);
            } else {
                printf("Insufficient quantity in stock.\n");
            }
            break;
        }
    }

    if(!found) {
        printf("Product with ID %d not found.\n", id);
    }
}

// Function to display transaction history
void displayTransactionHistory(Transaction *history, int historySize) {
    printf("\n=== Transaction History ===\n");
    for (int i = 0; i < historySize; i++) {
        printf("Timestamp: %s", ctime(&history[i].timestamp));
        printf("Type: %s\n", history[i].type);
        printf("Product ID: %d\n", history[i].product.id);
        printf("Product Name: %s\n", history[i].product.name);
        printf("Quantity: %d\n", history[i].quantity);
        printf("---------------------------------\n");
    }
}