#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;



class ProductClass {
public:
    int id;
    string name;
    string category;
    double price;
    int stock;

    ProductClass() {
        id = 0;
        name = "";
        category = "";
        price = 0.0;
        stock = 0;
    }

    ProductClass(int i, string n, string c, double p, int s) {
        id = i;
        name = n;
        category = c;
        price = p;
        stock = s;
    }

    string getStockStatus() {
        if (stock <= 0) return "Out of Stock";
        if (stock <= 5) return "Low Stock";
        return "In Stock";
    }
};

class ProductNode {
public:
    ProductClass data;
    ProductNode* next;

    ProductNode(ProductClass p) {
        data = p;
        next = nullptr;
    }
};

class CartItemClass {
public:
    int productId;
    string name;
    double price;
    int quantity;

    CartItemClass() {
        productId = 0;
        name = "";
        price = 0.0;
        quantity = 0;
    }

    CartItemClass(int id, string n, double p, int q) {
        productId = id;
        name = n;
        price = p;
        quantity = q;
    }
};

class CartNode {
public:
    CartItemClass data;
    CartNode* next;

    CartNode(CartItemClass c) {
        data = c;
        next = nullptr;
    }
};

class OrderClass {
public:
    int orderId;
    string customerName;
    string itemsSummary; // e.g., "Laptop x1, Mouse x2"
    double totalAmount;

    OrderClass() {
        orderId = 0;
        customerName = "";
        itemsSummary = "";
        totalAmount = 0.0;
    }

    OrderClass(int id, string cust, string summary, double total) {
        orderId = id;
        customerName = cust;
        itemsSummary = summary;
        totalAmount = total;
    }
};

class OrderNode {
public:
    OrderClass data;
    OrderNode* next;

    OrderNode(OrderClass o) {
        data = o;
        next = nullptr;
    }
};

// ===================== 2. PRODUCT LINKED LIST (HEAD & TAIL) =====================

class ProductList {
public:
    ProductNode* head;
    ProductNode* tail;

    ProductList() {
        head = nullptr;
        tail = nullptr;
    }

    ~ProductList() {
        clear();
    }

    void clear() {
        while (head != nullptr) {
            ProductNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    ProductNode* findById(int id) {
        ProductNode* curr = head;
        while (curr != nullptr) {
            if (curr->data.id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    bool addProduct(ProductClass p) {
        if (p.id <= 0 || p.price < 0 || p.stock < 0) {
            cout << "[Error] Invalid ID, price, or stock.\n";
            return false;
        }
        if (findById(p.id) != nullptr) {
            cout << "[Error] Duplicate Product ID!\n";
            return false;
        }

        ProductNode* newNode = new ProductNode(p);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        cout << "[Success] Product added successfully.\n";
        return true;
    }

    bool updateProduct(int id, string name, string cat, double price, int stock) {
        ProductNode* node = findById(id);
        if (node == nullptr) {
            cout << "[Error] Product not found.\n";
            return false;
        }
        if (price < 0 || stock < 0) {
            cout << "[Error] Price and stock cannot be negative.\n";
            return false;
        }
        node->data.name = name;
        node->data.category = cat;
        node->data.price = price;
        node->data.stock = stock;
        cout << "[Success] Product updated successfully.\n";
        return true;
    }

    bool deleteProduct(int id) {
        if (head == nullptr) {
            cout << "[Error] Product list is empty.\n";
            return false;
        }
        if (head->data.id == id) {
            ProductNode* temp = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
            delete temp;
            cout << "[Success] Product deleted successfully.\n";
            return true;
        }

        ProductNode* prev = head;
        ProductNode* curr = head->next;
        while (curr != nullptr) {
            if (curr->data.id == id) {
                prev->next = curr->next;
                if (curr == tail) tail = prev;
                delete curr;
                cout << "[Success] Product deleted successfully.\n";
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "[Error] Product ID not found.\n";
        return false;
    }

    void displayAll() {
        if (head == nullptr) {
            cout << "[Info] No products in catalog.\n";
            return;
        }
        cout << "\n---------------------------------------------------------------------------\n";
        cout << left << setw(8) << "ID"
             << setw(20) << "Name"
             << setw(16) << "Category"
             << right << setw(12) << "Price"
             << setw(8) << "Stock"
             << "   " << left << setw(14) << "Status" << "\n";
        cout << "---------------------------------------------------------------------------\n";
        ProductNode* curr = head;
        while (curr != nullptr) {
            cout << left << setw(8) << curr->data.id
                 << setw(20) << curr->data.name.substr(0, 19)
                 << setw(16) << curr->data.category.substr(0, 15)
                 << right << setw(12) << fixed << setprecision(2) << curr->data.price
                 << setw(8) << curr->data.stock
                 << "   " << left << setw(14) << curr->data.getStockStatus() << "\n";
            curr = curr->next;
        }
        cout << "---------------------------------------------------------------------------\n";
    }

    void searchProduct(int option, int id, string text) {
        if (head == nullptr) {
            cout << "[Info] Product catalog is empty.\n";
            return;
        }
        bool found = false;
        ProductNode* curr = head;
        while (curr != nullptr) {
            bool match = false;
            if (option == 1 && curr->data.id == id) match = true;
            if (option == 2 && curr->data.name == text) match = true;
            if (option == 3 && curr->data.category == text) match = true;

            if (match) {
                cout << "[Found] ID: " << curr->data.id
                     << " | Name: " << curr->data.name
                     << " | Category: " << curr->data.category
                     << " | Price: " << fixed << setprecision(2) << curr->data.price
                     << " | Stock: " << curr->data.stock
                     << " (" << curr->data.getStockStatus() << ")\n";
                found = true;
            }
            curr = curr->next;
        }
        if (!found) cout << "[Info] No matching product found.\n";
    }

    void sortProducts(int option) {
        if (head == nullptr || head->next == nullptr) {
            cout << "[Info] Not enough products to sort.\n";
            return;
        }
        for (ProductNode* i = head; i->next != nullptr; i = i->next) {
            for (ProductNode* j = head; j->next != nullptr; j = j->next) {
                bool swapNeeded = false;
                if (option == 1 && j->data.id > j->next->data.id) swapNeeded = true;
                if (option == 2 && j->data.name > j->next->data.name) swapNeeded = true;
                if (option == 3 && j->data.category > j->next->data.category) swapNeeded = true;
                if (option == 4 && j->data.price > j->next->data.price) swapNeeded = true;

                if (swapNeeded) {
                    ProductClass temp = j->data;
                    j->data = j->next->data;
                    j->next->data = temp;
                }
            }
        }
        cout << "[Success] Products sorted successfully.\n";
    }

    bool fileExists(string filename) {
        ifstream check(filename);
        bool exists = check.is_open();
        check.close();
        return exists;
    }

    void saveToFile(string filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "[Error] Could not open " << filename << " for saving.\n";
            return;
        }
        ProductNode* curr = head;
        while (curr != nullptr) {
            out << curr->data.id << "|"
                << curr->data.name << "|"
                << curr->data.category << "|"
                << curr->data.price << "|"
                << curr->data.stock << "\n";
            curr = curr->next;
        }
        out.close();
    }

    void loadFromFile(string filename) {
        if (!fileExists(filename)) {
            cout << "[Info] File \"" << filename << "\" does not exist yet.\n";
            return;
        }
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "[Error] Unable to open \"" << filename << "\".\n";
            return;
        }
        clear();
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, name, cat, priceStr, stockStr;
            getline(ss, idStr, '|');
            getline(ss, name, '|');
            getline(ss, cat, '|');
            getline(ss, priceStr, '|');
            getline(ss, stockStr, '|');

            ProductClass p(stoi(idStr), name, cat, stod(priceStr), stoi(stockStr));
            ProductNode* newNode = new ProductNode(p);
            if (head == nullptr) head = tail = newNode;
            else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        in.close();
        cout << "[Info] Loaded products from \"" << filename << "\".\n";
    }
};

// ===================== 3. SHOPPING CART LINKED LIST (HEAD & TAIL) =====================

class CartList {
public:
    CartNode* head;
    CartNode* tail;

    CartList() {
        head = nullptr;
        tail = nullptr;
    }

    ~CartList() {
        clear();
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void clear() {
        while (head != nullptr) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    void addToCart(ProductList& catalog, int productId, int qty) {
        if (qty <= 0) {
            cout << "[Error] Invalid quantity. Must be greater than 0.\n";
            return;
        }
        ProductNode* prod = catalog.findById(productId);
        if (prod == nullptr) {
            cout << "[Error] Product not found.\n";
            return;
        }
        if (prod->data.stock <= 0) {
            cout << "[Error] Product is Out of Stock!\n";
            return;
        }

        CartNode* curr = head;
        while (curr != nullptr) {
            if (curr->data.productId == productId) {
                if (curr->data.quantity + qty > prod->data.stock) {
                    cout << "[Error] Insufficient stock! Available: " << prod->data.stock << "\n";
                    return;
                }
                curr->data.quantity += qty;
                cout << "[Success] Cart quantity updated.\n";
                return;
            }
            curr = curr->next;
        }

        if (qty > prod->data.stock) {
            cout << "[Error] Insufficient stock! Available: " << prod->data.stock << "\n";
            return;
        }

        CartItemClass item(productId, prod->data.name, prod->data.price, qty);
        CartNode* newNode = new CartNode(item);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        cout << "[Success] Item added to cart.\n";
    }

    void removeFromCart(int productId) {
        if (head == nullptr) {
            cout << "[Error] Shopping cart is empty.\n";
            return;
        }
        if (head->data.productId == productId) {
            CartNode* temp = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
            delete temp;
            cout << "[Success] Item removed from cart.\n";
            return;
        }

        CartNode* prev = head;
        CartNode* curr = head->next;
        while (curr != nullptr) {
            if (curr->data.productId == productId) {
                prev->next = curr->next;
                if (curr == tail) tail = prev;
                delete curr;
                cout << "[Success] Item removed from cart.\n";
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "[Error] Item not in cart.\n";
    }

    void displayCart() {
        if (head == nullptr) {
            cout << "[Info] Shopping cart is empty.\n";
            return;
        }
        double total = 0;
        cout << "\n-----------------------------------------------------------------\n";
        cout << left << setw(8) << "ProdID"
             << setw(20) << "Name"
             << right << setw(8) << "Qty"
             << setw(12) << "Price"
             << setw(14) << "Subtotal" << "\n";
        cout << "-----------------------------------------------------------------\n";
        CartNode* curr = head;
        while (curr != nullptr) {
            double sub = curr->data.price * curr->data.quantity;
            total += sub;
            cout << left << setw(8) << curr->data.productId
                 << setw(20) << curr->data.name.substr(0, 19)
                 << right << setw(8) << curr->data.quantity
                 << setw(12) << fixed << setprecision(2) << curr->data.price
                 << setw(14) << fixed << setprecision(2) << sub << "\n";
            curr = curr->next;
        }
        cout << "-----------------------------------------------------------------\n";
        cout << "Total Cart Amount: " << fixed << setprecision(2) << total << "\n";
    }
};

// ===================== 4. ORDER QUEUE (FIFO) & PROCESSED HISTORY =====================

class OrderQueue {
public:
    OrderNode* front;         // Pending orders queue front
    OrderNode* rear;          // Pending orders queue rear
    OrderNode* completedHead; // Linked list for processed orders history
    OrderNode* completedTail;
    int nextOrderId;

    OrderQueue() {
        front = nullptr;
        rear = nullptr;
        completedHead = nullptr;
        completedTail = nullptr;
        nextOrderId = 1001;
    }

    ~OrderQueue() {
        clearPending();
        clearCompleted();
    }

    void clearPending() {
        while (front != nullptr) {
            OrderNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }

    void clearCompleted() {
        while (completedHead != nullptr) {
            OrderNode* temp = completedHead;
            completedHead = completedHead->next;
            delete temp;
        }
        completedTail = nullptr;
    }

    void enqueue(OrderClass o) {
        OrderNode* newNode = new OrderNode(o);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        if (o.orderId >= nextOrderId) nextOrderId = o.orderId + 1;
    }

    void addCompletedOrder(OrderClass o) {
        OrderNode* newNode = new OrderNode(o);
        if (completedHead == nullptr) {
            completedHead = completedTail = newNode;
        } else {
            completedTail->next = newNode;
            completedTail = newNode;
        }
        if (o.orderId >= nextOrderId) nextOrderId = o.orderId + 1;
    }

    bool placeOrder(ProductList& catalog, CartList& cart, string customerName) {
        if (cart.isEmpty()) {
            cout << "[Error] Cannot place order, cart is empty.\n";
            return false;
        }

        // Verify stock first
        CartNode* c = cart.head;
        while (c != nullptr) {
            ProductNode* p = catalog.findById(c->data.productId);
            if (p == nullptr || p->data.stock <= 0 || p->data.stock < c->data.quantity) {
                cout << "[Error] Insufficient/Out of stock for " << c->data.name << ".\n";
                return false;
            }
            c = c->next;
        }

        // Deduct stock and build order summary
        double total = 0;
        string summary = "";
        c = cart.head;
        while (c != nullptr) {
            ProductNode* p = catalog.findById(c->data.productId);
            p->data.stock -= c->data.quantity;
            total += c->data.price * c->data.quantity;

            if (!summary.empty()) summary += ", ";
            summary += c->data.name + " x" + to_string(c->data.quantity);
            c = c->next;
        }

        OrderClass newOrder(nextOrderId++, customerName, summary, total);
        enqueue(newOrder);
        cart.clear();

        cout << "[Success] Order #" << newOrder.orderId << " placed in FIFO queue! Total: "
             << fixed << setprecision(2) << total << "\n";
        return true;
    }

    bool processNextOrder() {
        if (front == nullptr) {
            cout << "[Error] Order queue is empty.\n";
            return false;
        }
        OrderNode* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;

        cout << "\n--- Processing Order #" << temp->data.orderId << " ---\n";
        cout << "Customer : " << temp->data.customerName << "\n";
        cout << "Items    : " << temp->data.itemsSummary << "\n";
        cout << "Total    : " << fixed << setprecision(2) << temp->data.totalAmount << "\n";
        cout << "Status   : Processed & Saved to History\n";

        // Move order record to completed orders history list before deleting queue node
        addCompletedOrder(temp->data);
        delete temp;
        return true;
    }

    void displayQueue() {
        if (front == nullptr) {
            cout << "[Info] Pending order queue is empty.\n";
            return;
        }
        cout << "\n--- Pending Orders (Front to Rear) ---\n";
        OrderNode* curr = front;
        while (curr != nullptr) {
            cout << "Order #" << curr->data.orderId
                 << " | Customer: " << curr->data.customerName
                 << " | Items: [" << curr->data.itemsSummary << "]"
                 << " | Total: " << fixed << setprecision(2) << curr->data.totalAmount << "\n";
            curr = curr->next;
        }
    }

    void displayProcessedOrders() {
        if (completedHead == nullptr) {
            cout << "[Info] No processed orders in history yet.\n";
            return;
        }
        cout << "\n--- Processed Orders History ---\n";
        OrderNode* curr = completedHead;
        while (curr != nullptr) {
            cout << "Order #" << curr->data.orderId
                 << " | Customer: " << curr->data.customerName
                 << " | Items: [" << curr->data.itemsSummary << "]"
                 << " | Total: " << fixed << setprecision(2) << curr->data.totalAmount
                 << " | Status: Processed\n";
            curr = curr->next;
        }
    }

    bool fileExists(string filename) {
        ifstream check(filename);
        bool exists = check.is_open();
        check.close();
        return exists;
    }

    void saveToFile(string pendingFile, string processedFile) {
        ofstream outPending(pendingFile);
        if (!outPending.is_open()) {
            cout << "[Error] Could not open " << pendingFile << " for saving.\n";
        } else {
            OrderNode* curr = front;
            while (curr != nullptr) {
                outPending << curr->data.orderId << "|"
                           << curr->data.customerName << "|"
                           << curr->data.itemsSummary << "|"
                           << curr->data.totalAmount << "\n";
                curr = curr->next;
            }
            outPending.close();
        }

        ofstream outProcessed(processedFile);
        if (!outProcessed.is_open()) {
            cout << "[Error] Could not open " << processedFile << " for saving.\n";
        } else {
            OrderNode* curr = completedHead;
            while (curr != nullptr) {
                outProcessed << curr->data.orderId << "|"
                             << curr->data.customerName << "|"
                             << curr->data.itemsSummary << "|"
                             << curr->data.totalAmount << "\n";
                curr = curr->next;
            }
            outProcessed.close();
        }
    }

    void loadFromFile(string pendingFile, string processedFile) {
        if (!fileExists(pendingFile)) {
            cout << "[Info] File \"" << pendingFile << "\" does not exist yet.\n";
        } else {
            ifstream inPending(pendingFile);
            if (inPending.is_open()) {
                clearPending();
                string line;
                while (getline(inPending, line)) {
                    if (line.empty()) continue;
                    stringstream ss(line);
                    string idStr, cust, summary, totalStr;
                    getline(ss, idStr, '|');
                    getline(ss, cust, '|');
                    getline(ss, summary, '|');
                    getline(ss, totalStr, '|');

                    OrderClass o(stoi(idStr), cust, summary, stod(totalStr));
                    enqueue(o);
                }
                inPending.close();
                cout << "[Info] Loaded pending orders from \"" << pendingFile << "\".\n";
            }
        }

        if (!fileExists(processedFile)) {
            cout << "[Info] File \"" << processedFile << "\" does not exist yet.\n";
        } else {
            ifstream inProcessed(processedFile);
            if (inProcessed.is_open()) {
                clearCompleted();
                string line;
                while (getline(inProcessed, line)) {
                    if (line.empty()) continue;
                    stringstream ss(line);
                    string idStr, cust, summary, totalStr;
                    getline(ss, idStr, '|');
                    getline(ss, cust, '|');
                    getline(ss, summary, '|');
                    getline(ss, totalStr, '|');

                    OrderClass o(stoi(idStr), cust, summary, stod(totalStr));
                    addCompletedOrder(o);
                }
                inProcessed.close();
                cout << "[Info] Loaded processed order history from \"" << processedFile << "\".\n";
            }
        }
    }
};

// ===================== 5. MAIN MENU =====================

int main() {
    ProductList catalog;
    CartList cart;
    OrderQueue orders;

    const string PRODUCTS_FILE = "products.txt";
    const string PENDING_ORDERS_FILE = "orders.txt";
    const string PROCESSED_ORDERS_FILE = "processed_orders.txt";

    catalog.loadFromFile(PRODUCTS_FILE);
    orders.loadFromFile(PENDING_ORDERS_FILE, PROCESSED_ORDERS_FILE);

    int choice;
    do {
        cout << "\n=== ONLINE SHOPPING & ORDER SYSTEM ===\n";
        cout << "1.  Add Product\n";
        cout << "2.  Update Product\n";
        cout << "3.  Delete Product\n";
        cout << "4.  Display All Products\n";
        cout << "5.  Search Product (1:ID, 2:Name, 3:Category)\n";
        cout << "6.  Sort Products (1:ID, 2:Name, 3:Category, 4:Price)\n";
        cout << "7.  Add Item to Cart\n";
        cout << "8.  Remove Item from Cart\n";
        cout << "9.  View Cart\n";
        cout << "10. Place Order (Enqueue)\n";
        cout << "11. Process Next Order (Dequeue)\n";
        cout << "12. View Pending Order Queue\n";
        cout << "13. View Processed Orders History\n";
        cout << "0.  Save & Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, stock;
            double price;
            string name, cat;
            cout << "Enter ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Name: "; getline(cin, name);
            cout << "Enter Category: "; getline(cin, cat);
            cout << "Enter Price: "; cin >> price;
            cout << "Enter Stock: "; cin >> stock;

            if (catalog.addProduct(ProductClass(id, name, cat, price, stock))) {
                catalog.saveToFile(PRODUCTS_FILE);
            }
        }
        else if (choice == 2) {
            int id, stock;
            double price;
            string name, cat;
            cout << "Enter Product ID to update: "; cin >> id;
            cin.ignore();
            cout << "Enter New Name: "; getline(cin, name);
            cout << "Enter New Category: "; getline(cin, cat);
            cout << "Enter New Price: "; cin >> price;
            cout << "Enter New Stock: "; cin >> stock;

            if (catalog.updateProduct(id, name, cat, price, stock)) {
                catalog.saveToFile(PRODUCTS_FILE);
            }
        }
        else if (choice == 3) {
            int id;
            cout << "Enter Product ID to delete: "; cin >> id;
            if (catalog.deleteProduct(id)) {
                catalog.saveToFile(PRODUCTS_FILE);
            }
        }
        else if (choice == 4) {
            catalog.displayAll();
        }
        else if (choice == 5) {
            int opt, id = 0;
            string text = "";
            cout << "Search by (1:ID, 2:Name, 3:Category): "; cin >> opt;
            if (opt == 1) {
                cout << "Enter ID: "; cin >> id;
            } else {
                cin.ignore();
                cout << "Enter Name/Category: "; getline(cin, text);
            }
            catalog.searchProduct(opt, id, text);
        }
        else if (choice == 6) {
            int opt;
            cout << "Sort by (1:ID, 2:Name, 3:Category, 4:Price): "; cin >> opt;
            catalog.sortProducts(opt);
        }
        else if (choice == 7) {
            int pid, qty;
            catalog.displayAll();
            cout << "Enter Product ID: "; cin >> pid;
            cout << "Enter Quantity: "; cin >> qty;
            cart.addToCart(catalog, pid, qty);
        }
        else if (choice == 8) {
            int pid;
            cart.displayCart();
            cout << "Enter Product ID to remove: "; cin >> pid;
            cart.removeFromCart(pid);
        }
        else if (choice == 9) {
            cart.displayCart();
        }
        else if (choice == 10) {
            string cust;
            cin.ignore();
            cout << "Enter Customer Name: "; getline(cin, cust);
            if (orders.placeOrder(catalog, cart, cust)) {
                catalog.saveToFile(PRODUCTS_FILE);
                orders.saveToFile(PENDING_ORDERS_FILE, PROCESSED_ORDERS_FILE);
            }
        }
        else if (choice == 11) {
            if (orders.processNextOrder()) {
                orders.saveToFile(PENDING_ORDERS_FILE, PROCESSED_ORDERS_FILE);
            }
        }
        else if (choice == 12) {
            orders.displayQueue();
        }
        else if (choice == 13) {
            orders.displayProcessedOrders();
        }
        else if (choice == 0) {
            catalog.saveToFile(PRODUCTS_FILE);
            orders.saveToFile(PENDING_ORDERS_FILE, PROCESSED_ORDERS_FILE);
            cout << "[Info] Saved all data and exiting...\n";
        }
        else {
            cout << "[Error] Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}