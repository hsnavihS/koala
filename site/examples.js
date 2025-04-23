// Auto-generated file. Do not edit directly.

const examples = {
  "combined": "func isPrime(n) {\n    if (n <= 1) return false;\n    if (n <= 3) return true;\n    \n    var i = 2;\n    while (i * i <= n) {\n        var quotient = n / i;\n        if (quotient * i == n) return false;\n        i = i + 1;\n    }\n    return true;\n}\n\nfunc findPrimes(limit) {\n    var count = 0;\n    var current = 2;\n    \n    while (count < limit) {\n        if (isPrime(current)) {\n            print current;\n            count = count + 1;\n        }\n        current = current + 1;\n    }\n}\n\nprint \"Primes:\";\nfindPrimes(10);\n\nfunc fibonacci(n) {\n    if (n <= 1) return n;\n    \n    var a = 0;\n    var b = 1;\n    var temp;\n    \n    for (var i = 2; i <= n; i = i + 1) {\n        temp = a + b;\n        a = b;\n        b = temp;\n    }\n    \n    return b;\n}\n\nprint \"\";\nprint \"Fibonacci numbers:\";\nfor (var i = 0; i < 10; i = i + 1) {\n    print fibonacci(i);\n}\n\nfunc factorial(n) {\n    if (n <= 1) return 1;\n    return n * factorial(n - 1);\n}\n\nprint \"\";\nprint \"Factorials:\";\nfor (var i = 0; i < 10; i = i + 1) {\n    print factorial(i);\n}\n",
  "control_flow": "var a = 1;\nvar b = 2;\n\nif (a < b) {\n    print \"a is less than b\";\n}\n\nif (a > b) {\n    print \"a is greater than b\";\n} else {\n    print \"a is not greater than b\";\n}\n\nif (a == 1) {\n    print \"a is 1\";\n} else if (a == 2) {\n    print \"a is 2\";\n} else {\n    print \"a is something else\";\n}\n\nif (true) {\n    print \"true is true\";\n}\n\nif (false) {\n    print \"false is true\";\n} else {\n    print \"false is false\";\n}\n\nif (nil) {\n    print \"nil is true\";\n} else {\n    print \"nil is false\";\n}\n\nif (0) {\n    print \"0 is true\";\n} else {\n    print \"0 is false\";\n}\n\nif (1) {\n    print \"1 is true\";\n} else {\n    print \"1 is false\";\n}\n\nif (\"\") {\n    print \"empty string is true\";\n} else {\n    print \"empty string is false\";\n}\n\nif (\"hello\") {\n    print \"non-empty string is true\";\n} else {\n    print \"non-empty string is false\";\n} ",
  "expressions": "print 42;\nprint \"Hello, world!\";\nprint true;\nprint false;\nprint nil;\n\nprint 1 + 2;\nprint 5 - 3;\nprint 2 * 3;\nprint 6 / 2;\n\nprint \"Hello\" + \" \" + \"World\";\nprint 1 + 2 + 3;\nprint 1 + 2 * 3;\nprint (1 + 2) * 3;\nprint -42;\nprint !true;\nprint !false;\nprint !nil;\nprint !0;\nprint !1;\n\nprint 1 == 1;\nprint 1 != 2;\nprint 1 < 2;\nprint 1 <= 2;\nprint 2 > 1;\nprint 2 >= 1;\n\nprint \"hello\" == \"hello\";\nprint \"hello\" != \"world\"; \n",
  "functions": "func sayHello() {\n    print \"Hello!\";\n}\n\nsayHello();\n\nfunc greet(name) {\n    print \"Hello, \" + name + \"!\";\n}\n\ngreet(\"World\");\ngreet(\"Koala\");\n\nfunc add(a, b) {\n    return a + b;\n}\n\nprint add(1, 2);\nprint add(3, 4);\n\nfunc factorial(n) {\n    if (n <= 1) return 1;\n    return n * factorial(n - 1);\n}\n\nprint factorial(5);\n\nfunc fib(n) {\n    if (n <= 1) return n;\n    return fib(n - 1) + fib(n - 2);\n}\n\nprint fib(10);\n\nfunc max(a, b) {\n    if (a > b) return a;\n    return b;\n}\n\nprint max(10, 20);\nprint max(30, 15);\n\nfunc min(a, b) {\n    if (a < b) return a;\n    return b;\n}\n\nprint min(10, 20);\nprint min(30, 15);\n\nfunc noReturn() {\n    print \"This function doesn't return anything\";\n}\n\nprint noReturn();\n\nfunc earlyReturn() {\n    print \"This will print\";\n    return;\n    print \"This won't print\";\n}\n\nearlyReturn(); ",
  "helloworld": "print \"Hello, world!\";\n",
  "loops": "var i = 0;\nwhile (i < 5) {\n\tprint i;\n\ti = i + 1;\n}\n\nprint \"\";\n\nfor (var j = 0; j < 5; j = j + 1) {\n\tprint j;\n}\n\nprint \"\";\n\nvar k = 0;\nwhile (k) {\n\tprint \"This should not print\";\n\tk = k + 1;\n}\n\nprint \"\";\n\nfor (var l = 0; l; l = l + 1) {\n\tprint \"This should not print\";\n}\n\nprint \"\";\n\nvar m = 5;\nwhile (m) {\n\tprint m;\n\tm = m - 1;\n}\n\nprint \"\";\n\nfor (var n = 5; n; n = n - 1) {\n\tprint n;\n}\n",
  "scoping": "var global = \"global\";\n\nfunc testScope() {\n    var local = \"local\";\n    print global;\n    print local;\n}\n\ntestScope();\nprint global;\n\n{\n    var block = \"block\";\n    print global;\n    print block;\n}\n\nprint global;\n\nfunc shadowTest() {\n    var global = \"shadowed\";\n    print global;\n}\n\nshadowTest();\nprint global;\n\nfunc nestedScope() {\n    var outer = \"outer\";\n    {\n        var inner = \"inner\";\n        print outer;\n        print inner;\n    }\n    print outer;\n}\n\nnestedScope();\n\nvar x = \"global x\";\nvar y = \"global y\";\n\n{\n    var x = \"local x\";\n    print x;\n    print y;\n}\n\nprint x;\nprint y;\n\nfunc scopeTest(a) {\n    var a = \"shadowed parameter\";\n    print a;\n}\n\nscopeTest(\"parameter\"); ",
  "variables": "var a = 1;\nprint a;\n\nvar b = \"hello\";\nprint b;\n\nvar c = true;\nprint c;\n\nvar d = nil;\nprint d;\n\na = 2;\nprint a;\n\nb = \"world\";\nprint b;\n\nc = false;\nprint c;\n\nd = 42;\nprint d;\n\nvar e = a + 1;\nprint e;\n\nvar f = b + \"!\";\nprint f;\n\nvar g = !c;\nprint g;\n\nvar h = e * 2;\nprint h;\n\nvar i = a;\nprint i;\n\ni = i + 1;\nprint i; "
};

function updateExampleSelect() {
  const select = document.getElementById('example-select');
  if (!select) return;
  
  while (select.options.length > 1) {
    select.remove(1);
  }
  
  Object.keys(examples).sort().forEach(name => {
    const option = document.createElement('option');
    option.value = name;
    option.textContent = name.replace(/_/g, ' ').replace(/w/g, l => l.toUpperCase());
    select.appendChild(option);
  });
}

window.examples = examples;
window.updateExampleSelect = updateExampleSelect;
