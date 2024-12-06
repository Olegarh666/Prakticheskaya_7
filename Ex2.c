#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_TRIES 100

struct msg_buffer {
    long msg_type;
    int guess;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    srand(time(NULL));

    key_t key = ftok("msgfile", 65); // Создание уникального ключа
    int msgid = msgget(key, 0666 | IPC_CREAT); // Создание очереди сообщений

    for (int game = 0; game < 10; game++) { // 10 игр
        int secret_number = rand() % N + 1; // Генерация случайного числа от 1 до N
        printf("Игрок 1 загадывает число: %d\n", secret_number);

        if (fork() == 0) { // Процесс игрока 2
            struct msg_buffer msg;
            int attempts = 0;

            while (1) {
                msgrcv(msgid, &msg, sizeof(msg.guess), 1, 0); // Получение предположения
                attempts++;
                printf("Игрок 2 пытается угадать: %d\n", msg.guess);

                if (msg.guess == secret_number) {
                    printf("Игрок 2 угадал число %d!\n", secret_number);
                    printf("Количество попыток: %d\n", attempts);
                    exit(0);
                } else {
                    printf("Игрок 2 не угадал. Попробуйте снова.\n");
                }
            }
        } else { // Процесс игрока 1
            struct msg_buffer msg;
            for (int i = 0; i < MAX_TRIES; i++) {
		msg.msg_type = 1;
		msg.guess = rand() % N + 1;
		msgsnd(msgid, &msg, sizeof(msg.guess), 0);
		sleep(1);
	    }
	    wait(NULL); // Ожидание завершения процесса игрока 2
                printf("Игра %d завершена.\n", game + 1);

                // Меняем местами игроков
                player_turn = (player_turn == 1) ? 2 : 1; // Меняем местами
                break; // Завершаем текущую игру и начинаем новую
	}
   }
   msgctl(msgid, IPC_RMID, NULL);
   return 0;
} 
