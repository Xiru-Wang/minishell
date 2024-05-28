# Test result

https://github.com/Xiru-Wang/minishell/assets/79924696/69c76cdf-c6ad-4711-998e-d89f984a56ff

# Goal
To create a miniature version of Bash, providing a hands-on understanding of how a Unix shell operates.

Challenges
-Lexical Analysis:
Design a simple lexer to tokenize the input command string. This involves breaking down the input into meaningful tokens, such as WORD, REDIR_IN, REDIR_OUT, APPEND, and HEREDOC, which the shell can then process.

-Parser:
Convert the token list into a command list saved in a linked list. Each command list includes the command, arguments, and a linked list with all the redirections.

-Redirections and Data Streams:
Comprehend and implement the way Bash handles input and output redirections. For example, Bash will first perform heredoc (if there is one), then data is supposed to go to the pipe. However, if there are redirect_out, data will go to the redirected file instead of the pipe(same for the redirect_in). This involves manipulating file descriptors to redirect data streams between commands and files.

-Forking and Piping:
Utilize fork to create child processes and pipe to enable inter-process communication. In our minishell, if there are multiple commands, the parent process will be in charge of creating child processes and controlling pipes to enable inter-process communication between child processes.

-Variable Expansion:
Manage edge cases related to variable expansion. This includes handling scenarios where variables may or may not be expanded, and ensuring the shell correctly interprets these cases. For example, handling strings with mixed quotes like hi$USER'$USER'"$USER"'"$?"'.

-Heredoc and Signal Handling:
Implement support for multiple heredoc (here-document) features, allowing users to input multi-line strings. Additionally, manage signal control to handle user interrupts (such as Ctrl+C, Ctrl+D) and ensure the shell responds appropriately to such signals.

# lexer
![14](https://github.com/Xiru-Wang/minishell/assets/79924696/a09c486f-d702-461a-af10-a3c9bbf9305d)
---
# parser
![15](https://github.com/Xiru-Wang/minishell/assets/79924696/e888b50c-6590-45ee-b395-91ac6c8c2c55)
---
# Multiple redirections
![16](https://github.com/Xiru-Wang/minishell/assets/79924696/3e500577-c034-446e-a019-560a758e459e)
---
# Expand complex string
![17](https://github.com/Xiru-Wang/minishell/assets/79924696/f484e554-50cb-496a-80e7-d8135c4de7eb)
