import qbs

Project {
    property bool enableProduct: true
    Product {
        name: "theProduct"
        condition: project.enableProduct
        Depends { name: "m" }
    }
}
