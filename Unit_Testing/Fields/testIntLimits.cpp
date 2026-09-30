#include "../../FieldR1.h"
#include "../../FieldR2.h"

int main()
{
    IntegerWithLimits intField1;
    IntegerWithLimits intField2;
    NameField nameField1("John", "Doe");

    intField1.generateRandom();

    string errorMsg;
    intField2.setMinAndMaxValues(-100, 100, errorMsg);
    intField2.setValue(25, errorMsg);

    int val1, val2;
    intField1.getValue(val1);
    intField2.getValue(val2);

    std::cout << "Integer Field 1 Value: " << val1 << std::endl;
    std::cout << "Integer Field 2 Value: " << val2 << std::endl;

    ComparisonType comparison;
    intField1.compare(intField2, comparison);

    if (comparison == ComparisonType::LOWER_THAN)
        std::cout << "Field 1 is less than Field 2" << std::endl;
    else if (comparison == ComparisonType::GREATER_THAN)
        std::cout << "Field 1 is greater than Field 2" << std::endl;
    else if (comparison == ComparisonType::EQUAL_TO)
        std::cout << "Field 1 is equal to Field 2" << std::endl;
    else
        std::cout << "Fields are not comparable" << std::endl;

    intField1.compare(nameField1, comparison);

    if (comparison == ComparisonType::LOWER_THAN)
        std::cout << "Field 1 is less than Name Field" << std::endl;
    else if (comparison == ComparisonType::GREATER_THAN)
        std::cout << "Field 1 is greater than Name Field" << std::endl;
    else if (comparison == ComparisonType::EQUAL_TO)
        std::cout << "Field 1 is equal to Name Field" << std::endl;
    else
        std::cout << "Fields are not comparable" << std::endl;

    return 0;
}