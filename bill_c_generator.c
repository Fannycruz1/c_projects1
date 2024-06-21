#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the items structure
struct items {
    char item[20];
    float price;
    int qty;
};

// Define the orders structure
struct orders {
    char customer[50];
    char date[50];
    int numofItems;
    struct items itm[50];
};

// Function to generate the bill header
void generateHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t    Fanny's Restaurant");
    printf("\n\t   ----------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("-------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n-------------------------------\n");
    printf("\n\n");
}

// Function to generate the bill body
void generateBillBody(char item[30], int qty, float price) {
    printf("%-16s", item);
    printf("%-16d", qty);
    printf("%.2f", qty * price);
    printf("\n");
}

// Function to generate the bill footer
void generateBillFooter(float total) {
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float cgst = 0.09 * netTotal;
    float grandTotal = netTotal + 2 * cgst;
    printf("\n--------------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", discount);
    printf("\n\t\t\t\t---------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    // using the same CGST variable for SGST variable 
    printf("\nCGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\nSGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\n--------------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n--------------------------------------------\n");
}

// Function to save an order to a file
void saveInvoice(struct orders ord) {
    FILE *fp = fopen("invoices.dat", "ab");
    if (fp != NULL) {
        fwrite(&ord, sizeof(struct orders), 1, fp);
        fclose(fp);
    } else {
        printf("Unable to open file for saving.\n");
    }
}

// Function to display the invoices
void showAllInvoices() {
    FILE *fp = fopen("invoices.dat", "rb");
    struct orders ord;
    int invoiceCount = 0;

    if (fp != NULL) {
        printf("\n============= All Invoices =============\n");
        while (fread(&ord, sizeof(struct orders), 1, fp)) {
            generateHeader(ord.customer, ord.date);
            float total = 0.0;
            for (int i = 0; i < ord.numofItems; i++) {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                total += ord.itm[i].qty * ord.itm[i].price;
            }
            generateBillFooter(total);
            invoiceCount++;
        }
        if (invoiceCount == 0) {
            printf("\nNo invoices found.\n");
        }
        fclose(fp);
    } else {
        printf("\nUnable to open invoice file.\n");
    }
}

// Function to search for an invoice by name
void searchInvoice(char customerName[50]) {
    FILE *fp = fopen("invoices.dat", "rb");
    struct orders ord;
    int invoiceFound = 0;

    if (fp != NULL) {
        while (fread(&ord, sizeof(struct orders), 1, fp)) {
            if (strcmp(ord.customer, customerName) == 0) {
                generateHeader(ord.customer, ord.date);
                float total = 0.0;
                for (int i = 0; i < ord.numofItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }
                generateBillFooter(total);
                invoiceFound = 1;
                break;
            }
        }
        if (!invoiceFound) {
            printf("\nInvoice for customer '%s' not found.\n", customerName);
        }
        fclose(fp);
    } else {
        printf("\nUnable to open invoice file.\n");
    }
}

// Main function for user interaction
int main() {
    int opt, n;
    struct orders ord;
    char searchCustomer[50];

    printf("\t=============FANNY'S RESTAURANT==============");
    printf("\n\nPlease enter your preferred operation");
    printf("\n\n1. Generate Invoice");
    printf("\n2. Show all Invoices");
    printf("\n3. Search Invoice");
    printf("\n4. Exit");

    printf("\n\nYour choice:\t");
    scanf("%d", &opt);
    fgetc(stdin); // Consume the newline character left in the buffer
    // using a switch operator for the options 
    switch (opt) {
        case 1:
            printf("\nEnter customer name:\t");
            fgets(ord.customer, 50, stdin);
            ord.customer[strlen(ord.customer) - 1] = 0; 
            // Remove the newline character
            strcpy(ord.date, __DATE__);
            printf("\nPlease enter the number of items:\t");
            scanf("%d", &n);
            ord.numofItems = n;

            for (int i = 0; i < n; i++) {
                fgetc(stdin); // Consume the newline character left in the buffer
                printf("\n\n");
                printf("Please enter the item %d:\t", i + 1);
                fgets(ord.itm[i].item, 20, stdin);
                ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0; // Remove the newline character
                printf("Please enter the quantity:\t");
                scanf("%d", &ord.itm[i].qty);
                printf("\nPlease enter the unit price:\t");
                scanf("%f", &ord.itm[i].price);
            }

            generateHeader(ord.customer, ord.date);

            float total = 0.0;
            for (int i = 0; i < ord.numofItems; i++) {
                generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                total += ord.itm[i].qty * ord.itm[i].price;
            }

            generateBillFooter(total);
            saveInvoice(ord);
            break;
        // display all invoices with function call 
        case 2:
            showAllInvoices();
            break;
        // call function to search for customer invoices
        case 3:
            printf("\nEnter customer name to search:\t");
            fgets(searchCustomer, 50, stdin);
            searchCustomer[strlen(searchCustomer) - 1] = 0; // Remove the newline character
            searchInvoice(searchCustomer);
            break;
        // exit program in case 4
        case 4:
            printf("\nExiting...\n");
            exit(0);
            break;
        // include default switch statement 
        default:
            printf("\nInvalid choice. Please try again.\n");
            break;
    }

    printf("\n\n");

    return 0;
}
