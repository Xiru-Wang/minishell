#include "./Unity/src/unity.h"
#include "../includes/minishell.h"

t_data *create_mock_data(void) {
    t_data *data = malloc(sizeof(t_data));
    if (!data) {
        return NULL;
    }
    // Initialize fields as necessary for testing
    data->line = NULL;  // Assuming not used in remove_quo_expand directly
    data->token_list = NULL;  // Assuming not used in remove_quo_expand directly
    data->var_name = NULL;  // Assuming not used in remove_quo_expand directly
    data->env = NULL;  // Assuming not used in remove_quo_expand directly
    data->cmd_list = NULL;  // Assuming not used in remove_quo_expand directly
    data->pid = NULL;  // Assuming not used in remove_quo_expand directly
    data->cmd_num = 0;  // Assuming not used in remove_quo_expand directly
    return data;
}

// Remember to free the mock data after each test
void free_mock_data(t_data *data) {
    if (data) {
        // Free any dynamically allocated fields if necessary
        free(data);
    }
}

void setUp(void) {
    // This is run before EACH test
}

void tearDown(void) {
    // This is run after EACH test
}

void test_remove_quo_expand_SingleQuotes(void) {
    t_data *data = create_mock_data();
    char *result = remove_quo_expand("hi'hi'", data);
    TEST_ASSERT_EQUAL_STRING("hihi", result);
    free(result);
    free_mock_data(data);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_remove_quo_expand_SingleQuotes);
    return UNITY_END();
}
