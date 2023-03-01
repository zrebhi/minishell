#include "../../includes/minishell.h"

void ft_print_env_sorted(t_env *env)
{
    // Copie de la liste chaînée
    t_env *copy = env;
    int count = 0;
    while (copy != NULL)
    {
        count++;
        copy = copy->next;
    }

    t_env **array = malloc(count * sizeof(t_env *));
    if (array == NULL)
    {
        printf("Erreur d'allocation de mémoire\n");
        return;
    }

    copy = env;
    for (int i = 0; i < count; i++)
    {
        array[i] = copy;
        copy = copy->next;
    }

    // Tri à bulles
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(array[j]->key, array[j+1]->key) > 0)
            {
                t_env *temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }

    // Affichage des éléments triés
    for (int i = 0; i < count; i++)
    {
        printf("%s=%s\n", array[i]->key, array[i]->value);
    }

    // Libération de la mémoire allouée
    free(array);
}

void    ft_built_in_export(t_env **head, char *string)
{
    char    **var_list;
    size_t  i;

    i = 1;
    var_list = ft_split_tokens(string, " ");
    if (!var_list[1])
        ft_print_env_sorted(*head);
    while (var_list[i])
    {
    	char *equal = ft_strchr(var_list[i], '=');
    	if (equal)
    	{
      		*equal = '\0';
      		*head = add_node(*head, var_list[i], equal + 1);
    	}
        else
            *head = add_node(*head, var_list[i], "''");
        i++;
    }
}