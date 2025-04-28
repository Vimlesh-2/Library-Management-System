#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BOOKS 60
#define MAX_STUDENTS 50
#define MAX_TITLE_LENGTH 100
#define MAX_CART_ITEMS 10
#define MAX_REDEEM_CODES 10
#define MAX_USED_CODES 10

struct book {
    char title[MAX_TITLE_LENGTH];
    float price;
    float hourly_rate;
    float renting_price;
    int quantity;
    char status;
};

int i, j;

struct redeem_code {
    char code[MAX_TITLE_LENGTH];
    float discount_percentage;
    float min_purchase_amount;
};

struct dashboard {
    int total_buyers;
    int total_readers;
    int total_renters;
    float total_sales;
    float total_income;
    float min_purchase_amount;
    struct redeem_code redeem_codes[MAX_REDEEM_CODES];
    int total_codes;
    float average_rating;
    int total_ratings;
};

struct student {
    int id;
    char name[MAX_TITLE_LENGTH];
    char location[MAX_TITLE_LENGTH];
    int seat_number;
    char remark;
    float total_cart_value;
    struct book cart[MAX_CART_ITEMS];
    int cart_count;
    char used_redeem_codes[MAX_USED_CODES][MAX_TITLE_LENGTH];
    int used_code_count;
    int rating;
    float last_transaction_amount;
};

// Function declarations
void show_predefined_books(struct book *library, int *total_books, struct dashboard *stats);
void add_books(struct book *library, int *total_books, struct dashboard *stats);
void remove_book(struct book *library, int *total_books);
void display_books(struct book *library, int total_books);
void declare_predefined_books(struct book *library, int *total_books, struct dashboard *stats);
void register_student(struct student *students, int *total_students);
int apply_redeem_code(struct student *student, struct dashboard *stats);
void checkout(struct student *student, struct dashboard *stats, struct book *library, int total_books);
void manager_dashboard(struct dashboard *stats, struct student *students, int total_students, struct book *library, int total_books);
void student_role(struct book *library, int total_books, struct student *students, int *total_students, struct dashboard *stats);
void deduct_quantity(struct book *library, int total_books, int index, int qty);
int collect_feedback(struct student *student, struct dashboard *stats);
void print_box(const char *text);
void add_predefined_books(struct book *library, int *total_books, struct dashboard *stats);
void remove_predefined_books(struct book *library, int *total_books);
void set_min_purchase_amount(struct dashboard *stats);
void cancel_order(struct student *student, struct book *library, int total_books);
void exit_program();
void sort_books_by_title(struct book *library, int total_books);

void register_student(struct student *students, int *total_students) {
    struct student new_student;
    printf("\nAvailable Student IDs: ");
    for (int i = 0; i < *total_students; i++) {
        printf("%d ", students[i].id);
    }
    printf("\nEnter Student ID: ");
    scanf("%d", &new_student.id);
    getchar();
    printf("Enter Student Name: ");
    fgets(new_student.name, MAX_TITLE_LENGTH, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0;
    printf("Enter Student Location: ");
    fgets(new_student.location, MAX_TITLE_LENGTH, stdin);
    new_student.location[strcspn(new_student.location, "\n")] = 0;
    printf("Enter Seat Number: ");
    scanf("%d", &new_student.seat_number);
    getchar();
    printf("Enter Category ('1' for Buyer, '2' for Reader, '3' for Renter): ");
    scanf("%c", &new_student.remark);
    getchar();
    new_student.total_cart_value = 0.0;
    new_student.cart_count = 0;
    new_student.used_code_count = 0;
    new_student.rating = 0;
    new_student.last_transaction_amount = 0.0;

    students[*total_students] = new_student;
    (*total_students)++;
}

// In the checkout function, store the transaction amount
void checkout(struct student *student, struct dashboard *stats, struct book *library, int total_books) {
    if (student->total_cart_value == 0) {
        printf("Your cart is empty. Please add items to your cart.\n");
        return;
    }
    printf("\nYour Cart Items:\n");
    for (int i = 0; i < student->cart_count; i++) {
        printf("%d. %s - $%.2f\n", i + 1, student->cart[i].title, student->cart[i].price);
    }
    float additional_charge = 0.0;
    if (student->remark == '2') {
        additional_charge = student->cart_count * library[0].hourly_rate;
        printf("Reading charge: $%.2f\n", additional_charge);
    } else if (student->remark == '3') {
        additional_charge = student->cart_count * library[0].renting_price;
        printf("Renting charge: $%.2f\n", additional_charge);
    }
    student->total_cart_value += additional_charge;
    printf("Total Cart Value: $%.2f\n", student->total_cart_value);
    char choice;
    printf("Do you have a redeem code? (y/n): ");
    scanf(" %c", &choice);
    getchar();
    if (choice == 'y' || choice == 'Y') {
        apply_redeem_code(student, stats);
    }

    if (student->total_cart_value < stats->min_purchase_amount) {
        printf("Minimum purchase amount not met. Please add more items to your cart.\n");
        return;
    }

    printf("Are you sure to checkout (y/n): ");
    scanf(" %c", &choice);
    getchar();
    if (choice != 'y' && choice != 'Y') {
        printf("Checkout cancelled.\n");
        return;
    }
    printf("Checkout successful! Total amount: $%.2f\n", student->total_cart_value);
    stats->total_sales += student->total_cart_value;

    // Store the transaction amount before resetting the cart
    student->last_transaction_amount = student->total_cart_value;

    // Update the appropriate counter based on student category
    if (student->remark == '1') {
        stats->total_buyers++;
    } else if (student->remark == '2') {
        stats->total_readers++;
    } else if (student->remark == '3') {
        stats->total_renters++;
    }

    // Update total income
    stats->total_income += student->total_cart_value;

    int rating_result = collect_feedback(student, stats);
    if (rating_result) {
        stats->average_rating = ((stats->average_rating * stats->total_ratings) + student->rating) / (stats->total_ratings + 1);
        stats->total_ratings++;
    }
    for (int i = 0; i < student->cart_count; i++) {
        for (int j = 0; j < total_books; j++) {
            if (strcmp(student->cart[i].title, library[j].title) == 0) {
                deduct_quantity(library, total_books, j, 1);
                break;
            }
        }
    }

    student->total_cart_value = 0.0;
    student->cart_count = 0;
}

int collect_feedback(struct student *student, struct dashboard *stats) {
    int rating;
    printf("Please provide your feedback rating (1 to 10): ");
    if (scanf("%d", &rating) == 1) {
        if (rating >= 1 && rating <= 10) {
            printf("Thank you for your feedback! You rated: %d\n", rating);
            student->rating = rating;
            return 1;
        } else {
            printf("Invalid rating! Please provide a rating between 1 and 10.\n");
        }
    } else {
        printf("Invalid input. Please enter a number between 1 and 10.\n");
        while (getchar() != '\n');
    }
    return 0;
}

// In the manager_dashboard function, display the last transaction amount
void manager_dashboard(struct dashboard *stats, struct student *students, int total_students, struct book *library, int total_books) {
    printf("-------------------------------------------------\n");
    printf("|            Manager Dashboard                  |\n");
    printf("-------------------------------------------------\n");
    printf("|Total Buyers: %d                               |\n", stats->total_buyers);
    printf("Total Readers: %d                               |\n", stats->total_readers);
    printf("Total Renters: %d                               |\n", stats->total_renters);
    printf("Total Sales: $%.2f                             |\n", stats->total_sales);
    printf("Total Income: $%.2f                            |\n", stats->total_income);
    printf("Minimum Purchase Amount: $%.2f                 |\n", stats->min_purchase_amount);
    printf("Average Student Rating: %.2f                   |\n", stats->average_rating);
    printf("-------------------------------------------------\n");
    printf("\nStudent Transaction Details:\n");
    printf("%-10s %-20s %-20s %-10s %-10s\n", "Student ID", "Student Name", "Transaction Amount", "Category", "Rating");

    for (int i = 0; i < total_students; i++) {
        // Display the last transaction amount instead of the current cart value
        printf("%-10d %-20s $%-20.2f %-10c %-10d\n",
               students[i].id,
               students[i].name,
               students[i].last_transaction_amount,
               students[i].remark,
               students[i].rating);
    }
    printf("\nBook Quantities:\n");
    printf("%-50s %-10s\n", "Book Title", "Quantity");
    for (int i = 0; i < total_books; i++) {
        printf("%-50s %-10d\n", library[i].title, library[i].quantity);
    }

    int search_id;
    printf("\nDo you want to search for a student? (1 for Yes, 0 for No): ");
    scanf("%d", &search_id);
    if (search_id) {
        printf("Enter Student ID to search: ");
        scanf("%d", &search_id);
        for (int i = 0; i < total_students; i++) {
            if (students[i].id == search_id) {
                printf("Student Found: ID: %d, Name: %s, Category: %c, Cart Value: $%.2f\n", students[i].id, students[i].name, students[i].remark, students[i].total_cart_value);
                break;
            }
        }
    }
}

void student_role(struct book *library, int total_books, struct student *students, int *total_students, struct dashboard *stats) {
    int student_id;
    printf("Enter your student ID to proceed: ");

    if (scanf("%d", &student_id) != 1) {
        printf("Invalid input. Please enter a valid Student ID.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    int student_index = -1;
    for (int i = 0; i < *total_students; i++) {
        if (students[i].id == student_id) {
            student_index = i;
            break;
        }
    }

    if (student_index == -1) {
        printf("Student ID not found. Please register first.\n");
        return;
    }

    printf("Welcome, Student %s!\n", students[student_index].name);

    int choice;
    while (1) {
        printf("-------------------------------------------------\n");
        printf("| Welcome to Library Management System (Student)|\n");
        printf("-------------------------------------------------\n");
        printf("|1. View Available Books                        |\n");
        printf("|2. Add Book to Cart                            |\n");
        printf("|3. Remove Book from Cart                       |\n");
        printf("|4. Apply Redeem Code                           |\n");
        printf("|5. Checkout                                    |\n");
        printf("|6. Cancel Order                                |\n");
        printf("|7. Exit                                        |\n");
        printf("-------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            display_books(library, total_books);
        } else if (choice == 2) {
            int book_index, qty;
            printf("Enter the index of the book to add to cart: ");
            if (scanf("%d", &book_index) == 1) {
                book_index--;
                printf("Enter quantity: ");
                if (scanf("%d", &qty) == 1) {
                    if (book_index >= 0 && book_index < total_books && library[book_index].quantity >= qty && library[book_index].status == 'A') {
                        int already_in_cart = 0;
                        for (int j = 0; j < students[student_index].cart_count; j++) {
                            if (strcmp(students[student_index].cart[j].title, library[book_index].title) == 0) {
                                already_in_cart = 1;
                                break;
                            }
                        }
                        if (!already_in_cart) {
                            students[student_index].cart[students[student_index].cart_count] = library[book_index];
                            students[student_index].cart_count++;
                            students[student_index].total_cart_value += library[book_index].price * qty;
                            deduct_quantity(library, total_books, book_index, qty);
                            printf("Book added to cart successfully!\n");
                        } else {
                            printf("Book is already in the cart.\n");
                        }
                    } else {
                        printf("Invalid book index, insufficient quantity, or book not available.\n");
                    }
                } else {
                    printf("Invalid input for quantity.\n");
                    while (getchar() != '\n');
                }
            } else {
                printf("Invalid input for book index.\n");
                while (getchar() != '\n');
            }
            while (getchar() != '\n');
        } else if (choice == 3) {
            int book_index;
            printf("Enter the index of the book to remove from cart: ");
            if (scanf("%d", &book_index) == 1) {
                book_index--;
                if (book_index >= 0 && book_index < students[student_index].cart_count) {
                    students[student_index].total_cart_value -= students[student_index].cart[book_index].price;
                    for (int i = book_index; i < students[student_index].cart_count - 1; i++) {
                        students[student_index].cart[i] = students[student_index].cart[i + 1];
                    }
                    students[student_index].cart_count--;
                    printf("Book removed from cart successfully!\n");
                } else {
                    printf("Invalid book index.\n");
                }
            } else {
                printf("Invalid input for book index.\n");
                while (getchar() != '\n');
            }
            while (getchar() != '\n');
        } else if (choice == 4) {
            apply_redeem_code(&students[student_index], stats);
        } else if (choice == 5) {
            checkout(&students[student_index], stats, library, total_books);
        } else if (choice == 6) {
            cancel_order(&students[student_index], library, total_books);
        } else if (choice == 7) {
            break;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void print_box(const char *text) {
    int len = strlen(text);
    printf("\n");
    for (i = 0; i < len + 6; i++) {
        printf("-");
    }
    printf("\n");
}

void add_predefined_books(struct book *library, int *total_books, struct dashboard *stats) {
    int existing_books = *total_books;
    declare_predefined_books(library + existing_books, total_books, stats);
    *total_books = 30 + existing_books;
    printf("Predefined Books added successfully!\n");
    int add_more;
    printf("Do you want to add some more books in predefined list? (1 for Yes , 0 for No)");
    scanf("%d", &add_more);
    getchar();
    if (add_more == 1) {
        add_books(library, total_books, stats);
    }
}

void remove_predefined_books(struct book *library, int *total_books) {
    *total_books = 0;
    printf("Predefined books removed successfully!\n");
}

void set_min_purchase_amount(struct dashboard *stats) {
    printf("Enter minimum purchase amount : ");
    if (scanf("%f", &stats->min_purchase_amount) == 1) {
        printf("Minimum purchase amount set to $%.2f.\n", stats->min_purchase_amount);
    } else {
        printf("Invalid input. Please enter a valid number.\n");
    }
    getchar();
}

void cancel_order(struct student *student, struct book *library, int total_books) {
    if (student->cart_count == 0) {
        printf("Your cart is already empty.\n");
        return;
    }

    char choice;
    printf("Are you sure you want to cancel your order? (y/n): ");
    scanf(" %c", &choice);
    getchar();

    if (choice == 'y' || choice == 'Y') {
        for (int i = 0; i < student->cart_count; i++) {
            for (int j = 0; j < total_books; j++) {
                if (strcmp(student->cart[i].title, library[j].title) == 0) {
                    library[j].quantity += 1;
                    break;
                }
            }
        }
        student->total_cart_value = 0.0;
        student->cart_count = 0;
        printf("Order cancelled successfully!\n");
    } else {
        printf("Order cancellation aborted.\n");
    }
}

void exit_program() {
    printf("Exiting the program. Goodbye!\n");
    exit(0);
}

// Add these missing function implementations

int apply_redeem_code(struct student *student, struct dashboard *stats) {
    char code[MAX_TITLE_LENGTH];
    printf("Enter redeem code: ");
    fgets(code, MAX_TITLE_LENGTH, stdin);
    code[strcspn(code, "\n")] = 0;
    for (int i = 0; i < student->used_code_count; i++) {
        if (strcmp(student->used_redeem_codes[i], code) == 0) {
            printf("You have already used this redeem code.\n");
            return 0;
        }
    }
    for (int i = 0; i < stats->total_codes; i++) {
        if (strcmp(stats->redeem_codes[i].code, code) == 0) {
            if (student->total_cart_value >= stats->redeem_codes[i].min_purchase_amount) {
                float discount = (student->total_cart_value * stats->redeem_codes[i].discount_percentage) / 100;
                student->total_cart_value -= discount;
                printf("Redeem code applied! Discount: $%.2f, New total: $%.2f\n", discount, student->total_cart_value);
                strcpy(student->used_redeem_codes[student->used_code_count], code);
                student->used_code_count++;
                return 1;
            } else {
                printf("Minimum purchase amount not met for this redeem code.\n");
                return 0;
            }
        }
    }
    printf("Invalid redeem code.\n");
    return 0;
}

void deduct_quantity(struct book *library, int total_books, int index, int qty) {
    library[index].quantity -= qty;
    if (library[index].quantity <= 0) {
        library[index].status = 'N';
    }
}

void display_books(struct book *library, int total_books) {
    printf("\n%-5s %-50s %-10s %-15s %-15s %-15s %-15s\n",
           "Index", "Book Title", "Price", "Quantity", "Status", "Hourly Rate", "Renting Price");
    for (int i = 0; i < total_books; i++) {
        printf("%-5d %-50s $%-10.2f %-15d %-15c $%-15.2f $%-15.2f\n",
               i + 1, library[i].title, library[i].price, library[i].quantity, library[i].status, library[i].hourly_rate, library[i].renting_price);
    }
}

void declare_predefined_books(struct book *library, int *total_books, struct dashboard *stats) {
    char *titles[30] = {
        "C Programming", "Data Structures", "Algorithms", "Operating Systems", "Database Systems",
        "Computer Networks", "Discrete Mathematics", "Software Engineering", "Artificial Intelligence",
        "Machine Learning", "Deep Learning", "Computer Architecture", "Cyber Security", "Digital Logic",
        "Linear Algebra", "Theory of Computation", "Database Management", "Computer Graphics",
        "Design Patterns", "Java Programming", "Python Programming", "JavaScript Basics",
        "PHP Programming", "Web Development", "Mobile App Development", "Cloud Computing",
        "Blockchain Technology", "IoT", "Embedded Systems", "Network Security"
    };

    for (int i = 0; i < 30; i++) {
        strcpy(library[i].title, titles[i]);
        library[i].price = (rand() % 901) + 100;
        library[i].hourly_rate = library[i].price * 0.1;
        library[i].renting_price = library[i].price * 0.2;
        if (i == 0) {
            library[i].quantity = 1;
        } else {
            library[i].quantity = 20;
        }
        library[i].status = 'A';
    }
}

void add_books(struct book *library, int *total_books, struct dashboard *stats) {
    int book_count;
    printf("Enter number of books to add: ");
    scanf("%d", &book_count);
    getchar();
    for (int i = *total_books; i < *total_books + book_count; i++) {
        printf("Enter book title: ");
        fgets(library[i].title, MAX_TITLE_LENGTH, stdin);
        library[i].title[strcspn(library[i].title, "\n")] = 0;
        printf("Enter book price ($100 - $1000): ");
        scanf("%f", &library[i].price);
        getchar();
        while (library[i].price < 100 || library[i].price > 1000) {
            printf("Invalid price. Enter book price ($100 - $1000): ");
            scanf("%f", &library[i].price);
            getchar();
        }
        printf("Enter book quantity: ");
        scanf("%d", &library[i].quantity);
        getchar();
        printf("Enter book status ('A' for Available, 'N' for Not available): ");
        scanf("%c", &library[i].status);
        getchar();
        library[i].hourly_rate = library[i].price * 0.1;
        library[i].renting_price = library[i].price * 0.2;
    }
    *total_books += book_count;
    sort_books_by_title(library, *total_books);
}

// Add a main function since the linker is looking for it
int main() {
    struct book library[MAX_BOOKS];
    struct student students[MAX_STUDENTS];
    struct dashboard stats = {0};
    int total_books = 0, total_students = 0, choice;
    srand(time(NULL));
    while (1) {
        printf("-------------------------------------------------\n");
        printf("| Welcome to Library Management System (Manager)|\n");
        printf("-------------------------------------------------\n");
        printf("| 1. Use Predefined List of Books                |\n");
        printf("| 2. Create Your Own Book List                   |\n");
        printf("| 3. Remove a Book                               |\n");
        printf("| 4. Set Redeem Codes                            |\n");
        printf("| 5. Register Student                            |\n");
        printf("| 6. View Dashboard                              |\n");
        printf("| 7. Set Minimum Purchase Amount                 |\n");
        printf("| 8. Add Predefined Books                        |\n");
        printf("| 9. Remove Predefined Books                     |\n");
        printf("| 10. Exit (Switch to Student Role)              |\n");
        printf("| 11. Exit Program                               |\n");
        printf("-------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                show_predefined_books(library, &total_books, &stats);
                break;
            case 2:
                add_books(library, &total_books, &stats);
                break;
            case 3:
                remove_book(library, &total_books);
                break;
            case 4:
                printf("Enter the number of redeem codes to add: ");
                scanf("%d", &stats.total_codes);
                getchar();

                for (int i = 0; i < stats.total_codes; i++) {
                    printf("\nEnter Redeem Code: ");
                    fgets(stats.redeem_codes[i].code, MAX_TITLE_LENGTH, stdin);
                    stats.redeem_codes[i].code[strcspn(stats.redeem_codes[i].code, "\n")] = 0;

                    printf("Enter discount percentage (e.g., 10 for 10%%): ");
                    scanf("%f", &stats.redeem_codes[i].discount_percentage);

                    printf("Enter minimum purchase amount for the redeem code: ");
                    scanf("%f", &stats.redeem_codes[i].min_purchase_amount);
                    getchar();
                }
                break;
            case 5:
                register_student(students, &total_students);
                break;
            case 6:
                manager_dashboard(&stats, students, total_students, library, total_books);
                break;
            case 7:
                set_min_purchase_amount(&stats);
                break;
            case 8:
                add_predefined_books(library, &total_books, &stats);
                break;
            case 9:
                remove_predefined_books(library, &total_books);
                break;
            case 10:
                student_role(library, total_books, students, &total_students, &stats);
                break;
            case 11:
                exit_program();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void show_predefined_books(struct book *library, int *total_books, struct dashboard *stats) {
    int existing_books = *total_books;
    if (existing_books == 0) {
        declare_predefined_books(library, total_books, stats);
        *total_books = 30;
    } else {
        // Check if predefined books are already loaded
        int predefined_books_exist = 0;
        for (int i = 0; i < existing_books; i++) {
            if (strcmp(library[i].title, "C Programming") == 0) {
                predefined_books_exist = 1;
                break;
            }
        }

        if (predefined_books_exist) {
            printf("Predefined books are already loaded in the system.\n");
        } else {
            declare_predefined_books(library + existing_books, total_books, stats);
            *total_books = 30 + existing_books;
        }
    }
    int add_more;
    printf("Do you want to add more books to the predefined list? (1 for Yes, 0 for No): ");
    scanf("%d", &add_more);
    getchar();
    if (add_more == 1) {
        add_books(library, total_books, stats);
    }
    print_box("Predefined List of Books");
    sort_books_by_title(library, *total_books);
    display_books(library, *total_books);
}

void sort_books_by_title(struct book *library, int total_books) {
    struct book temp;
    for (int i = 0; i < total_books - 1; i++) {
        for (int j = i + 1; j < total_books; j++) {
            if (strcmp(library[i].title, library[j].title) > 0) {
                temp = library[i];
                library[i] = library[j];
                library[j] = temp;
            }
        }
    }
}

void remove_book(struct book *library, int *total_books) {
    int book_index;
    printf("Enter the index of the book to remove (1 to %d): ", *total_books);
    scanf("%d", &book_index);
    book_index--;
    if (book_index >= 0 && book_index < *total_books) {
        for (int i = book_index; i < *total_books - 1; i++) {
            library[i] = library[i + 1];
        }
        (*total_books)--;
        printf("Book removed successfully!\n");
    } else {
        printf("Invalid index!\n");
    }
}
