#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

static t_data *test_data = NULL;

void setup_generate_cmds() {
    test_data = malloc(sizeof(t_data));
    test_data->token_list = NULL;
    test_data->cmd_list = NULL;
    test_data->env = NULL;
}

void teardown_generate_cmds() {
    if (test_data) {
        free_cmd_list(&test_data->cmd_list);
        free_token_list(&test_data->token_list);
        free(test_data);
        test_data = NULL;
    }
}

static t_token *add_token(t_token **head, char *value, t_type type) {
    t_token *new_token = malloc(sizeof(t_token));
    new_token->value = strdup(value);
    new_token->type = type;
    new_token->next = *head;
    if (*head != NULL) (*head)->prev = new_token;
    *head = new_token;
    return new_token;
}

/* static void free_tokens(t_token *head) { */
/*     while (head) { */
/*         t_token *next = head->next; */
/*         free(head->value); */
/*         free(head); */
/*         head = next; */
/*     } */
/* } */

Test(minishell_commands, simple_command, .init = setup_generate_cmds, .fini = teardown_generate_cmds) {
    add_token(&test_data->token_list, "hello", WORD);
    add_token(&test_data->token_list, "echo", WORD);

    test_data->cmd_list = generate_cmds(&test_data->token_list, test_data);
    
    cr_assert_not_null(test_data->cmd_list, "Command list should not be null.");
    cr_assert_str_eq(test_data->cmd_list->s[0], "echo", "First command should be 'echo'. Expected 'echo', got '%s'", test_data->cmd_list->s[0]);
    cr_assert_str_eq(test_data->cmd_list->s[1], "hello", "Second command should be 'hello'. Expected 'hello', got '%s'", test_data->cmd_list->s[1]);
}

Test(minishell_commands, command_with_redirection, .init = setup_generate_cmds, .fini = teardown_generate_cmds) {
    add_token(&test_data->token_list, "file.txt", WORD);
    add_token(&test_data->token_list, ">", REDIR_OUT);
    add_token(&test_data->token_list, "echo", WORD);

    test_data->cmd_list = generate_cmds(&test_data->token_list, test_data);
    
    cr_assert_not_null(test_data->cmd_list, "Command list should not be null.");
    cr_assert_not_null(test_data->cmd_list->io_list, "IO list should not be null.");
    cr_assert_eq(test_data->cmd_list->io_list->type, REDIR_OUT, "Redirection type should be REDIR_OUT. Expected %d, got %d", REDIR_OUT, test_data->cmd_list->io_list->type);
    cr_assert_str_eq(test_data->cmd_list->io_list->filename, "file.txt", "Output should be redirected to 'file.txt'. Expected 'file.txt', got '%s'", test_data->cmd_list->io_list->filename);
}
