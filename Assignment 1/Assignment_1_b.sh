#!/bin/bash

validatePhone()
{
    phone=$1
    if [[ $phone =~ ^[0-9]{10}$ ]]; then
        return 0
    else
        return 1
    fi
}

validateEmail()
{
    email=$1
    if [[ $email =~ ^[A-Za-z0-9.-_]+@[A-Za-z.-]+\.[A-Za-z]{2,}$ ]]; then
        return 0
    else
        return 1
    fi
}

checkUnique()
{
    phone=$1
    email=$2
    addressBook=$3

    if tail -n +2 "$addressBook" | grep -q "$phone"; then
        echo "Phone Number Already Exists!"
        return 1
    fi

    if tail -n +2 "$addressBook" | grep -q "$email"; then
        echo "Email Already Exists!"
        return 1
    fi
    
    return 0
}

createAddressBook()
{
    read -p "Enter Name Of Address Book: " nameAddressBook

    if [ -f "$nameAddressBook" ]; then
        echo "Address Book With Same Name Already Exists!"
    else
        touch "$nameAddressBook"
        printf "%-20s\t%-12s\t%-30s\t%-50s\n" "Name" "Phone" "Email" "Address" > "$nameAddressBook"
        echo "Address Book '$nameAddressBook' Created Successfully!"
    fi
}

insertRecord()
{
    read -p "Enter Name Of Address Book: " nameAddressBook

    if [ ! -f "$nameAddressBook" ]; then
        echo "Address Book '$nameAddressBook' Does Not Exist!"
    else
        read -p "Enter Name: " name

        read -p "Enter Phone: " phone
        if ! validatePhone "$phone"; then
            echo "Invalid Phone Number!"
            return 1
        fi

        read -p "Enter Email: " email
        if ! validateEmail "$email"; then
            echo "Invalid email!"
            return 1
        fi

        if ! checkUnique "$phone" "$email" "$nameAddressBook"; then
            return 1
        fi

        read -p "Enter Address: " address

        printf "%-20s\t%-12s\t%-30s\t%-50s\n" "$name" "$phone" "$email" "$address" >> "$nameAddressBook"

        echo "Record Inserted Successfully!"
    fi
}

displayAddressBook()
{
    read -p "Enter Name Of Address Book: " nameAddressBook

    if [ ! -f "$nameAddressBook" ]; then
        echo "Address Book '$nameAddressBook' Does Not Exist!"
    else
        cat "$nameAddressBook"
    fi
}

modifyRecord()
{
    read -p "Enter Name Of Address Book: " nameAddressBook

    if [ ! -f "$nameAddressBook" ]; then
        echo "Address Book '$nameAddressBook' Does Not Exist!"
        return 1
    fi

    read -p "Enter Phone Number Of The Record To Be Modified: " searchPhone

    if grep -q "$searchPhone" "$nameAddressBook"; then
        echo "Record Found! Enter New Details:"
        read -p "Enter Name: " name

        read -p "Enter Phone: " phone
        if ! validatePhone "$phone"; then
            echo "Invalid Phone Number!"
            return 1
        fi
        if [ "$phone" != "$searchPhone" ]; then
            if tail -n +2 "$nameAddressBook" | grep -q "$phone"; then
                echo "Phone Number Already Exists!"
                return 1
            fi
        fi

        read -p "Enter Email: " email
        if ! validateEmail "$email"; then
            echo "Invalid email!"
            return 1
        fi
        currentEmail=$(grep "$searchPhone" "$nameAddressBook" | cut -f3)
        if [ "$email" != "$currentEmail" ]; then
            if tail -n +2 "$nameAddressBook" | grep -q "$email"; then
                echo "Email Already Exists!"
                return 1
            fi
        fi

        read -p "Enter Address: " address

        newRecord=$(printf "%-20s\t%-12s\t%-30s\t%-50s\n" "$name" "$phone" "$email" "$address")

        sed -i "/$searchPhone/c\\$newRecord" "$nameAddressBook"
        echo "Record Modified Successfully!"
    else
        echo "Record Does Not Exist!"
    fi
}

deleteRecord()
{
    read -p "Enter Name Of Address Book: " nameAddressBook

    if [ ! -f "$nameAddressBook" ]; then
        echo "Address Book '$nameAddressBook' Does Not Exist!"
        return 1
    fi

    read -p "Enter Phone Number Of The Record To Be Deleted: " searchPhone

    if grep -q "$searchPhone" "$nameAddressBook"; then
        # Display the record before deletion
        echo "Found Record:"
        grep "$searchPhone" "$nameAddressBook"
        
        read -p "Are You Sure You Want To Delete This Record? (y/n): " confirm
        
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            sed -i "/$searchPhone/d" "$nameAddressBook"
            echo "Record Deleted Successfully!"
        else
            echo "Deletion Cancelled!"
        fi
    else
        echo "Record Does Not Exist!"
    fi
}

while [ true ]
do
    echo
    echo "**********Address Book Menu**********"
    echo "1. Create A New Address Book"
    echo "2. Insert A Record"
    echo "3. Display Address Book"
    echo "4. Modify A Record"
    echo "5. Delete A Record"
    echo "6. Exit"
    echo
    read -p "Enter Your Choice: " choice
    echo

    case $choice in
        1)
            createAddressBook
            ;;
        2)
            insertRecord
            ;;
        3)
            displayAddressBook
            ;;
        4)
            modifyRecord
            ;;
        5)
            deleteRecord
            ;;
        6)
            exit 0
            ;;
        *)
            echo "Invalid Choice!"
            ;;
    esac
done


