# 🔍 Parser Implementations in C

This repository contains C programs to implement various parsing techniques used in Compiler Design:

- ✅ LL(1) Parser
- ✅ FIRST and FOLLOW set computation
- ✅ Operator Precedence Parser
- ✅ LEADING and TRAILING set computation

---

## 📁 Files Included

| File                         | Description |
|------------------------------|-------------|
| `ll1_parser.c`               | Implements an LL(1) parser using predictive parsing table |
| `first_follow.c`            | Calculates the FIRST and FOLLOW sets of a given grammar |
| `operator_precedence.c`     | Parses expressions using operator precedence relations |
| `leading_trailing.c`        | Computes the LEADING and TRAILING sets for operator precedence grammars |

---

## ⚙️ How to Compile and Run

### Step 1: Compile

Use `gcc` to compile any of the source files. Example:

```bash
gcc ll1_parser.c -o ll1_parser
