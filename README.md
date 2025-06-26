#  tinysh — A Minimal Unix Shell

> `tinysh` is a lightweight interactive shell written in C, powered by GNU Readline, designed by [Mehra Ayisha](https://github.com/mehraayisha) with simplicity, learning, and elegance in mind 🤍

---

## ✨ Features

- [x] Run Unix commands (`ls`, `pwd`, etc.)
- [x] Built-in commands: `cd`, `exit`, `help`, `clear`, `version`
- [x] Command history with up/down arrow keys (Readline)
- [x] Persistent history saved to `.tinysh_history`
- [x] External manual loaded from `tinysh.man`
- [x] **Pipe support** (`|`) — chain multiple commands like `ls -l | grep .c | wc -l`


---

## 📦 How to Build

Make sure you have `gcc` and `libreadline-dev`:

```bash
sudo apt install build-essential libreadline-dev

