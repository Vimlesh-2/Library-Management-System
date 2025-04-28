# Library-Management-System (In 'C' Program)
 


________________________________________
(C Programming)
This document outlines the design and functionalities of a Library Management System implemented in C programming. The system facilitates the efficient management of books, students, transactions (buying, renting, and reading), reporting, and integrating special offer systems such as redeem codes. Below is a detailed breakdown of the key features and functionalities:
________________________________________
1. Getting the List of Books in the Library
• Adding Books: The system allows the library owner or manager to either create a custom list of books or use a predefined list.
• Setting Prices: The manager can define prices for each book, including rental and reading charges.
• Editing Books: Books in the system can be edited—titles can be renamed, prices adjusted, or books removed from the catalog.
• Setting Reading Rates: The manager can assign hourly reading rates for books that are read within the library.
• Sorting Books: The books are sorted alphabetically (A-Z) to help students find the book they need more easily.
• Handling Multi-Part Books: Books with multiple parts (e.g., Book 1 and Book 2) are listed as a pair in the system.
• Renting Price for Books: Each book has a set price for one-day rentals, which the manager can define.
________________________________________
2. Student Registration and Allocation
• Getting Student Details: The system prompts for student registration by gathering their name, location, seat number, and other details.
• Assigning Unique ID: A unique student ID is generated and assigned to each student for identification.
________________________________________
3. Displaying Books with Pricing
• Menu of Books: A list of all available books with their respective prices, hourly reading rates, and availability status is displayed for students.
• Student Choice: Students can choose between buying a book (“B”), reading it inside the library (“R”), or renting it for home reading (“HR”).
• Reading Pricing: If a student chooses to read a book inside the library, the hourly reading charge is shown. The student can specify how many hours they wish to read.
• Availability Check: The system checks whether the selected book is available in the library. If unavailable, alternative suggestions are made.
• Handling Unavailable Books: If a book is out of stock, the system informs the student and provides a list of available alternatives.
________________________________________
4. Student Checkout and Exit
• Exit Query: After browsing or making a purchase, students are asked if they wish to exit the library (Y/N).
• Checkout Cart: If a student purchases a book, they are presented with a summary of their cart, including book prices, and given the option to continue shopping or remove books from the cart before finalizing the purchase.
• Book Availability Confirmation: The system asks whether the student found the books they were looking for (Y/N).
• Rating the Library: Upon exit, students are invited to rate their library experience on a scale from 1 to 10.
________________________________________
5. Remarking Students
• Categorizing Students: Students are categorized based on their activity:
•	“B” for buyers (students who purchase books)
•	“R” for readers (students reading in the library)
•	“HR” for home readers (students who rent books for home reading)
________________________________________
6. Daily Sales and Reading Report
• Manager/Owner Report: The system generates a daily report detailing total income, sales, books rented or read inside the library, and overall transactions.
________________________________________
7. Handling Unavailable Books
• Unavailability Notification: If a book is unavailable, the system notifies the manager with the book’s name and the quantity needed.
• Book Addition: The manager is prompted to either add the book to the catalog or delay the decision. Options to "Add," "Remind Later," or "Decline" are available.
________________________________________
8. Special Offers for Buyers
• Offer Code Inquiry: The system prompts students buying books to inquire whether they have a redeem code (Y/N).
• Validating Code: If an offer code is provided, the system validates it and applies any applicable discounts to the student's total cart value.
• Minimum Purchase Amount: If applicable, the system checks if the student’s total cart value meets the minimum purchase requirement for the redeem code. Students are informed if they are eligible or not.
• Option to Buy More: If a student's cart value does not meet the redeem code requirement, they are offered the option to add more books to their cart to become eligible for the discount.
________________________________________
9. Home Reader Checkout
• Collecting Home Reader Details: When a student chooses to rent a book for home reading, the system collects the number of days the student wishes to rent the book, as well as their mobile number, address, and other details for contact and shipping.
________________________________________
10. Daily Sales and Rent Report
• Sales Summary: At the end of the day, the system generates a detailed report on total sales, rented books, and books read inside the library. This includes quantities and total income earned from these transactions.
________________________________________
11. Managing Book Quantities
• Updating Book Availability: After each transaction (buying, renting, or reading), the system updates the quantity of books and reflects the current stock. If books run out, they are marked as "Out of stock" or displayed as unavailable until restocked.
________________________________________
Conclusion
The Library Management System, implemented in C programming, is designed to streamline library operations, improve student and library staff interactions, and automate daily tasks. This system efficiently handles book management, student registration, transactions (buying, renting, and reading), daily reports, special offers, and the handling of unavailable books. By integrating features like redeem codes for discounts, real-time inventory management, and categorization of student activities, the system ensures smooth and organized management of library resources while providing a user-friendly interface for both students and library managers.
________________________________________
This revised synopsis reflects the new functionalities added to your system, including the redeem code system, manager dashboard, student categorization, and more. The system is now more versatile, offering students the ability to access special offers and giving managers the tools to track transactions and sales efficiently.
Checkout Project 

