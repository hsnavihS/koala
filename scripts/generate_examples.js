const fs = require('fs');
const path = require('path');

const testsDir = path.join(__dirname, '..', 'tests');
const outputFile = path.join(__dirname, '..', 'site', 'examples.js');

const examples = {};
const files = fs.readdirSync(testsDir);
files.forEach(file => {
  if (file.endsWith('.kol')) {
    const filePath = path.join(testsDir, file);
    const content = fs.readFileSync(filePath, 'utf8');
    const name = file.replace('.kol', '');
    examples[name] = content;
  }
});

const jsContent = `// Auto-generated file. Do not edit directly.

const examples = ${JSON.stringify(examples, null, 2)};

function updateExampleSelect() {
  const select = document.getElementById('example-select');
  if (!select) return;
  
  while (select.options.length > 1) {
    select.remove(1);
  }
  
  Object.keys(examples).sort().forEach(name => {
    const option = document.createElement('option');
    option.value = name;
    option.textContent = name.replace(/_/g, ' ').replace(/\b\w/g, l => l.toUpperCase());
    select.appendChild(option);
  });
}

window.examples = examples;
window.updateExampleSelect = updateExampleSelect;
`;

fs.writeFileSync(outputFile, jsContent);
console.log(`Generated ${outputFile} with ${Object.keys(examples).length} examples`); 
