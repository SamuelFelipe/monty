#include "monty.h"

int a = 0;
int *status = &a;

void pall(stack_t **head, unsigned int line)
{
	stack_t *tp = NULL;
	(void)line;

	if (!head || !*head)
		return;

	tp = *head;

	for (; tp;)
	{
		printf("%d\n", tp->n);
		tp = tp->next;
	}
}

void free_stack(stack_t *head)
{
	stack_t *f = head;

	while (head)
	{
		head = head->next;
		free(f);
		f = head;
	}
	free(f);
}

void push(stack_t **head, unsigned int line, char *data)
{
	stack_t *new = NULL;

	if (!data)
	{
		dprintf(STDERR_FILENO, "L%d: usage: push integer\n", line);
		*status = 1;
	}
	else
	{
		new = malloc(sizeof(stack_t));
		if (!new)
		{
			dprintf(STDERR_FILENO, "L%d: usage: push integer\n", line);
			*status = 1;
			return;
		}
		new->n = atoi(data);
		new->next = *head;
		new->prev = NULL;
		if (*head != NULL)
			(*head)->prev = new;
		*head = new;
	}
}

void s_and_e(data_t *data)
{
	stack_t **head = NULL;
	int i;
	char *opcode = NULL, *s = " \n\t";
	
	instruction_t list[] = {
		{"pall", pall},
		{NULL, NULL}
	};


	head = malloc(sizeof(stack_t));
	if (!head)
	{
		free_data_t(data);
		exit(EXIT_FAILURE);
	}

	while(fgets(data->line_buffer, 1024, data->file))
	{
		opcode = strtok(data->line_buffer, s);
		if (strcmp("push", opcode) == 0)
			push(head, data->line++, strtok(NULL, s));
		else
		{
			for (i = 0; list[i].opcode; i++)
			{
				if (strcmp(opcode, list[i].opcode) == 0)
				{
					list[i].f(head, data->line++);
					break;
				}
			}
		}
		if (a == 1)
		{
			free_stack(*head);
			free_data_t(data);
			exit(EXIT_FAILURE);
		}
	}
	free_stack(*head);
	free(head);
}
