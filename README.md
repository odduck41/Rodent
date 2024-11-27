# It's a simple programming language✨

Our beautiful mascot: [Katarni](https://github.com/Katarni)  
<img src="img/Katarni.jpg" alt="Katarni" height=300px>

It called Rodent, because we(me, cause @Artem-627 doesn't care) wanted to name it after a squirrel, quoll, etc. But I undecided.

Example:

```
import "file.rod"
/*
  comment
*/

// also comment
func gcd(int a, int b) -> int {
    while (a && b) {
        if (a > b) {
          a %= b;
        } else {
          b %= a;
        }
    }
    return a + b;
}

func sum (int a, int b) -> int {
    return a + b;
}

func main() -> int  {
    int a = 3, b = 4;
    gcd(a, b);
    sum(2, b);
    do {
        a -= 1;
    } while (a != 1);
}

/*
  We use wchar_t, so...
  Мы используем wchar_t, так что...
  Utimur wchar_t, ita...
  Mir benotzen wchar_t, also...
אנו משתמשים ב-wchar_t, כך...
  Wir verwenden wchar_t, also...
  நாங்கள் wchar_t ஐப் பயன்படுத்துகிறோம், எனவே...
*/

```

Syntax: [here](https://github.com/odduck41/Rodent/blob/main/docs/Language.md)🎈


