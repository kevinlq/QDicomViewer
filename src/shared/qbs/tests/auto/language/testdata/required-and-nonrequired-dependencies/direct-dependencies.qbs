import qbs

Product {
    Depends { name: "failing-validation"; required: false }
    Depends { name: "failing-validation" }
}
