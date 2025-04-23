// Pressing tab shouldn't take the user to the next HTML element when writing code
document.getElementById("code").addEventListener("keydown", function (e) {
  if (e.key === "Tab") {
    e.preventDefault();
    const start = this.selectionStart;
    const end = this.selectionEnd;
    const value = this.value;

    // tab is 2 spaces
    this.value = value.substring(0, start) + "  " + value.substring(end);
    this.selectionStart = this.selectionEnd = start + 2;
  }
});

const ansiUp = new AnsiUp();
let koalaOutput = "";

function updateOutput(text) {
  koalaOutput += text + "\n";
  const outputElement = document.getElementById("output");
  outputElement.innerHTML = ansiUp.ansi_to_html(koalaOutput.trim());

  // If output has errors, add error class to the output element
  const lowerText = koalaOutput.toLowerCase();
  if (lowerText.includes("error")) {
    outputElement.classList.add("error");
  } else {
    outputElement.classList.remove("error");
  }
}

var Module = {
  preRun: [],
  postRun: [],
  print: (text) => {
    updateOutput(text);
  },
  printErr: (text) => {
    updateOutput(text);
  },
  onRuntimeInitialized: function () {
    document.getElementById("run-btn").disabled = false;

    window.runKoala = function () {
      koalaOutput = "";
      document.getElementById("output").textContent = koalaOutput;

      const code = document.getElementById("code").value;
      const result = Module.ccall(
        "interpret", // name of C++ function
        "void", // return type
        ["string"], // argument types
        [code], // argument values
      );
    };

    document.getElementById("run-btn").onclick = runKoala;
  },
};

// Update the example select dropdown when the page loads
document.addEventListener('DOMContentLoaded', () => {
  updateExampleSelect();
});

document
  .getElementById("example-select")
  .addEventListener("change", function (e) {
    const code = window.examples[e.target.value];
    if (code) {
      document.getElementById("code").value = code;
    }
  });

document
  .getElementById("clear-btn")
  .addEventListener("click", function () {
    document.getElementById("code").value = "";
  }); 