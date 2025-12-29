# Syntax Highlighting Specification

## Color Scheme
| Element Type          | Color     | Hex       | ncurses | Description |
|-----------------------|-----------|-----------|---------|-------------|
| Operators             | Red       | `#FF4444` | 1       | `+ - * / = % ++ --` |
| Logical Operators     | Pink      | `#FF66B2` | 2       | `== != && \|\| < >` |
| Functions             | Yellow    | `#FFD700` | 3       | Function names & scopes |
| Keywords              | Purple    | `#AA66CC` | 4       | `if while for return` |
| Strings               | Green     | `#44AA44` | 5       | `"strings" 'chars'` |
| Template Placeholders | Dark Green| `#2E8B57` | 6       | `${}` in template strings |
| Comments              | Gray      | `#888888` | 7       | `// /* */ #` |
| Numbers               | Blue      | `#4444FF` | 8       | `123 3.14 0xFF` |

## Scope Rules
- **Functions**: Name + parentheses + entire body scope
- **Control Flow**: Keyword + condition parentheses + body scope
- **Strings**: Delimiters + content (different shades for templates)