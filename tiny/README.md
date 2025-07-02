# tinysh — A Minimal Unix Shell

> `tinysh` is a lightweight interactive shell written in C, powered by GNU Readline, designed by [Mehra Ayisha](https://github.com/mehraayisha) with simplicity, learning, and elegance in mind 🤍

---

## ✨ Features

- [x] Run Unix commands (`ls`, `pwd`, etc.)
- [x] Built-in commands: `cd`, `exit`, `help`, `clear`, `version`
- [x] Command history with up/down arrow keys (Readline)
- [x] Persistent history saved to `.tinysh_history`
- [x] External manual loaded from `tinysh.man`
- [x] **Pipe support** (`|`) — chain commands like `ls -l | grep .c | wc -l`
- [x] 💬 **AI Assistant Support** — Ask Linux & Bash questions with `/ask`
- [x] Powered by **Groq's LLaMA 3 API** for fast responses
- [x] Timeout-safe queries to avoid blocking

---

## 💬 AI Assistant (/ask)

You can query the shell for help using the `/ask` command:

```bash
tiny>> /ask What is a socket in Linux?
