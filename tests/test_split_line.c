#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

static t_data *test_data = NULL;

static void free_tokens(t_token *token_list)
{
    t_token *current = token_list;
    while (current != NULL) {
        t_token *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void setup_test_tokens(void)
{
    t_data *data = malloc(sizeof(t_data));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    data->token_list = NULL;
    data->env = NULL;
    test_data = data;
}

void teardown_test_tokens(void)
{
    t_data *data = (t_data *)test_data;
    free_tokens(data->token_list);
    free(data);
}

// Test Cases
Test(split_line, simple_command, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo Hello, World!";
    char *actual_value;
    char *expected_value;
    
    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

	// 1. TOKEN
    t_token *token = data->token_list;
    cr_assert_not_null(token, "Token list should not be null.");
    cr_assert_eq(token->type, WORD, "Token type should be WORD.");
    actual_value = token->value;
    expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);

	// 2. TOKEN
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    cr_assert_eq(token->type, WORD, "Token type should be WORD.");
    actual_value = token->value;
    expected_value = "Hello,";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);

	// 3. TOKEN
	token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    cr_assert_eq(token->type, WORD, "Token type should be WORD.");
    actual_value = token->value;
    expected_value = "World!";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
}

Test(split_line, command_with_pipe, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "cat file.txt | grep 'hello'";
    char *actual_value;
    char *expected_value;

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'cat'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    actual_value = token->value;
    expected_value = "cat";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for 'file.txt'
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "file.txt";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Second token type should be WORD.");

    // Check for '|'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "|";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, PIPE, "Third token type should be PIPE.");

    // Check for 'grep'
    token = token->next;
    cr_assert_not_null(token, "Fourth token should not be null.");
    actual_value = token->value;
    expected_value = "grep";
    cr_assert_str_eq(actual_value, expected_value, "Fourth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Fourth token type should be WORD.");

    // Check for ''hello''
    token = token->next;
    cr_assert_not_null(token, "Fifth token should not be null.");
    actual_value = token->value;
    expected_value = "'hello'";
    cr_assert_str_eq(actual_value, expected_value, "Fifth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Fifth token type should be QUO.");
}

Test(split_line, command_with_options, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo ls -l";
    char *actual_value;
    char *expected_value;

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'echo'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    actual_value = token->value;
    expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for 'ls'
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "ls";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Second token type should be WORD.");

    // Check for '-l'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "-l";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Third token type should be WORD.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, unclosed_quotes, .init = setup_test_tokens, .fini = teardown_test_tokens, .signal = SIGSEGV) {
    t_data *data = (t_data *)test_data;
    char *input = "echo \"Hello World";

    // Redirect stdout to capture the output
    cr_redirect_stdout();

    // Assuming split_line is supposed to handle the input and print to stdout
    int result = split_line(input, &data->token_list, data);

    // Check the exit status
    cr_assert_eq(result, 0, "Expected split_line to return 0 on unclosed quotes.");

    // Use Criterion's function to get the captured output from stdout
    FILE *output_file = cr_get_redirected_stdout();
    cr_assert_not_null(output_file, "Failed to capture stdout");

    // Read the captured output into a buffer
    char output[100];
    fgets(output, sizeof(output), output_file);

    // Remove the newline character from the output
    output[strcspn(output, "\n")] = '\0';

    // Validate the output
    char *expected_output = "unclosed quote";
    cr_assert_str_eq(output, expected_output, "Error message mismatch.\nActual: '%s'\nExpected: '%s'", output, expected_output);
}

Test(split_line, complex_command_with_redirection, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "grep 'pattern' file1.txt > output.txt";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'grep'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "grep";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for ''pattern''
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "'pattern'";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Second token type should be WORD.");

    // Check for 'file1.txt'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "file1.txt";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Third token type should be WORD.");

    // Check for '>'
    token = token->next;
    cr_assert_not_null(token, "Fourth token should not be null.");
    actual_value = token->value;
    expected_value = ">";
    cr_assert_str_eq(actual_value, expected_value, "Fourth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, REDIR_OUT, "Fourth token type should be GREATER.");

    // Check for 'output.txt'
    token = token->next;
    cr_assert_not_null(token, "Fifth token should not be null.");
    actual_value = token->value;
    expected_value = "output.txt";
    cr_assert_str_eq(actual_value, expected_value, "Fifth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Fifth token type should be WORD.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, command_with_multiple_redirections, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "cat < input.txt > output.txt > error.txt";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'cat'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "cat";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for '<'
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "<";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, REDIR_IN, "Second token type should be REDIR_IN.");

    // Check for 'input.txt'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "input.txt";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Third token type should be WORD.");

    // Check for '>'
    token = token->next;
    cr_assert_not_null(token, "Fourth token should not be null.");
    actual_value = token->value;
    expected_value = ">";
    cr_assert_str_eq(actual_value, expected_value, "Fourth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, REDIR_OUT, "Fourth token type should be REDIR_OUT.");

    // Check for 'output.txt'
    token = token->next;
    cr_assert_not_null(token, "Fifth token should not be null.");
    actual_value = token->value;
    expected_value = "output.txt";
    cr_assert_str_eq(actual_value, expected_value, "Fifth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);

    // Check for '2>'
    token = token->next;
    cr_assert_not_null(token, "Sixth token should not be null.");
    actual_value = token->value;
    expected_value = ">";
    cr_assert_str_eq(actual_value, expected_value, "Sixth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, REDIR_OUT, "Sixth token type should be REDIR_OUT.");

    // Check for 'error.txt'
    token = token->next;
    cr_assert_not_null(token, "Seventh token should not be null.");
    actual_value = token->value;
    expected_value = "error.txt";
    cr_assert_str_eq(actual_value, expected_value, "Seventh token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, command_with_single_quotes, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo 'Hello World'";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'echo'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for 'Hello World'
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "'Hello World'";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Second token type should be QUO.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, command_with_heredoc, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "cat << EOF | grep 'pattern' EOF";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'cat'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "cat";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for '<<'
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "<<";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, HEREDOC, "Second token type should be HEREDOC.");

    // Check for 'EOF'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "EOF";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Third token type should be WORD.");

    // Check for '|'
    token = token->next;
    cr_assert_not_null(token, "Fourth token should not be null.");
    actual_value = token->value;
    expected_value = "|";
    cr_assert_str_eq(actual_value, expected_value, "Fourth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, PIPE, "Fourth token type should be PIPE.");

    // Check for 'grep'
    token = token->next;
    cr_assert_not_null(token, "Fifth token should not be null.");
    actual_value = token->value;
    expected_value = "grep";
    cr_assert_str_eq(actual_value, expected_value, "Fifth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Fifth token type should be WORD.");

    // Check for 'pattern'
    token = token->next;
    cr_assert_not_null(token, "Sixth token should not be null.");
    actual_value = token->value;
    expected_value = "'pattern'";
    cr_assert_str_eq(actual_value, expected_value, "Sixth token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Sixth token type should be QUO.");

    // Check for 'EOF'
    token = token->next;
    cr_assert_not_null(token, "Third token should not be null.");
    actual_value = token->value;
    expected_value = "EOF";
    cr_assert_str_eq(actual_value, expected_value, "Third token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Third token type should be WORD.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, command_with_quotes_and_spaces, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo \"Hello   World\"";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'echo'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for "Hello   World"
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "\"Hello   World\"";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Second token type should be QUU.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}

Test(split_line, command_with_empty_quotes, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo \"\"";

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'echo'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    char *actual_value = token->value;
    char *expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for empty string
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");
    actual_value = token->value;
    expected_value = "\"\"";
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, QUO, "Second token type should be QUO.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}


Test(split_line, command_with_variable_expansion, .init = setup_test_tokens, .fini = teardown_test_tokens) {
    t_data *data = (t_data *)test_data;
    char *input = "echo $HOME";
    char *actual_value;
    char *expected_value;

    int result = split_line(input, &data->token_list, data);
    cr_assert_eq(result, 1, "Expected split_line to succeed.");

    // Check for 'echo'
    t_token *token = data->token_list;
    cr_assert_not_null(token, "First token should not be null.");
    actual_value = token->value;
    expected_value = "echo";
    cr_assert_str_eq(actual_value, expected_value, "First token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "First token type should be WORD.");

    // Check for expanded value of $HOME
    token = token->next;
    cr_assert_not_null(token, "Second token should not be null.");

    // Depending on your implementation, the expected value may vary
    actual_value = token->value;
    expected_value = "$HOME";  // or "$HOME" if expansion happens later
    cr_assert_str_eq(actual_value, expected_value, "Second token mismatch.\nActual: '%s'\nExpected: '%s'", actual_value, expected_value);
    cr_assert_eq(token->type, WORD, "Second token type should be WORD.");

    // Check for NULL
    token = token->next;
    cr_assert_null(token, "Token list should end with NULL.");
}
