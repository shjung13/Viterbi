#define _USE_MATH_DEFINES
#define MAX_HMM_NUM 6
#define N_PDF		10
#define N_DIMENSION	39

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<io.h>
#include<math.h>
#include"hmm.h"

typedef struct {
	char name[10]; // Word
	hmmType hmm[6];
	int hmm_index[6];
	int hmm_num;
	int start_state_num;
	int end_state_num;
} Word;

Word word_model[13];

float transmatrix[127][127] = { 0.0, };

float bi_prob[13][13] = { 0, };

float input[700][39] = { 0, };

int data_size = 0;

int max_state[400][127] = { 0, }; // save max state

float state_prob[400][127] = { 0, }; // to calculate prob

int backprob_state[400] = { 0, };

int find_word(char *buf) {
	if (strcmp(buf, "<s>") == 0)
		return 0;
	else if (strcmp(buf, "eight") == 0)
		return 1;
	else if (strcmp(buf, "five") == 0)
		return 2;
	else if (strcmp(buf, "four") == 0)
		return 3;
	else if (strcmp(buf, "nine") == 0)
		return 4;
	else if (strcmp(buf, "oh") == 0)
		return 5;
	else if (strcmp(buf, "one") == 0)
		return 6;
	else if (strcmp(buf, "seven") == 0)
		return 7;
	else if (strcmp(buf, "six") == 0)
		return 8;
	else if (strcmp(buf, "three") == 0)
		return 9;
	else if (strcmp(buf, "two") == 0)
		return 10;
	else if (strcmp(buf, "zero") == 0)
		return 11;
	else if (strcmp(buf, "eh") == 0)
		return 12;
	else
		return -1;
}

int find_phone(char *buf) {
	if (strcmp(buf, "f") == 0)
		return 0;
	else if (strcmp(buf, "k") == 0)
		return 1;
	else if (strcmp(buf, "n") == 0)
		return 2;
	else if (strcmp(buf, "r") == 0)
		return 3;
	else if (strcmp(buf, "s") == 0)
		return 4;
	else if (strcmp(buf, "t") == 0)
		return 5;
	else if (strcmp(buf, "v") == 0)
		return 6;
	else if (strcmp(buf, "w") == 0)
		return 7;
	else if (strcmp(buf, "z") == 0)
		return 8;
	else if (strcmp(buf, "ah") == 0)
		return 9;
	else if (strcmp(buf, "ao") == 0)
		return 10;
	else if (strcmp(buf, "ay") == 0)
		return 11;
	else if (strcmp(buf, "eh") == 0)
		return 12;
	else if (strcmp(buf, "ey") == 0)
		return 13;
	else if (strcmp(buf, "ih") == 0)
		return 14;
	else if (strcmp(buf, "iy") == 0)
		return 15;
	else if (strcmp(buf, "ow") == 0)
		return 16;
	else if (strcmp(buf, "th") == 0)
		return 17;
	else if (strcmp(buf, "uw") == 0)
		return 18;
	else if (strcmp(buf, "sil") == 0)
		return 19;
	else if (strcmp(buf, "sp") == 0)
		return 20;
	else {
		return -1;
	}
}

int find_model(int state_num) {
	if ((0 < state_num) && (state_num < 4))
		return 0;
	else if ((3 < state_num) && (state_num < 11))
		return 1;
	else if ((10 < state_num) && (state_num < 21))
		return 2;
	else if ((20 < state_num) && (state_num < 31))
		return 3;
	else if ((30 < state_num) && (state_num < 41))
		return 4;
	else if ((40 < state_num) && (state_num < 45))
		return 5;
	else if ((44 < state_num) && (state_num < 55))
		return 6;
	else if ((54 < state_num) && (state_num < 71))
		return 7;
	else if ((70 < state_num) && (state_num < 84))
		return 8;
	else if ((83 < state_num) && (state_num < 94))
		return 9;
	else if ((93 < state_num) && (state_num < 101))
		return 10;
	else if ((100 < state_num) && (state_num < 114))
		return 11;
	else if ((113 < state_num) && (state_num < 128))
		return 12;
	else
		return -1;
}

int get_start_state(char *buf) {
	if (strcmp(buf, "<s>") == 0)
		return 1;
	else if (strcmp(buf, "eight") == 0)
		return 4;
	else if (strcmp(buf, "five") == 0)
		return 11;
	else if (strcmp(buf, "four") == 0)
		return 21;
	else if (strcmp(buf, "nine") == 0)
		return 31;
	else if (strcmp(buf, "oh") == 0)
		return 41;
	else if (strcmp(buf, "one") == 0)
		return 45;
	else if (strcmp(buf, "seven") == 0)
		return 55;
	else if (strcmp(buf, "six") == 0)
		return 71;
	else if (strcmp(buf, "three") == 0)
		return 84;
	else if (strcmp(buf, "two") == 0)
		return 94;
	else if (strcmp(buf, "zero") == 0)
		return 101;
	else
		return -1;
}

int get_end_state(char *buf) {
	if (strcmp(buf, "<s>") == 0)
		return 3;
	else if (strcmp(buf, "eight") == 0)
		return 10;
	else if (strcmp(buf, "five") == 0)
		return 20;
	else if (strcmp(buf, "four") == 0)
		return 30;
	else if (strcmp(buf, "nine") == 0)
		return 40;
	else if (strcmp(buf, "oh") == 0)
		return 44;
	else if (strcmp(buf, "one") == 0)
		return 54;
	else if (strcmp(buf, "seven") == 0)
		return 70;
	else if (strcmp(buf, "six") == 0)
		return 83;
	else if (strcmp(buf, "three") == 0)
		return 93;
	else if (strcmp(buf, "two") == 0)
		return 100;
	else if (strcmp(buf, "zero") == 0)
		return 113;
	else
		return -1;
}

void make_word_model() {
	FILE *dict = fopen("dictionary.txt", "r");
	char buf[13][30];
	int index = 0;

	while (!feof(dict))  // 파일의 끝이 아니라면
	{

		fgets(buf[index], 30, dict);  // 최대 80칸짜리 한줄 읽기
		if (buf[index][strlen(buf[index]) - 1] == '\n')
			buf[index][strlen(buf[index]) - 1] = '\0';
		char *ptr = "0000000";
		ptr = strtok(buf[index], "\t");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
		strcpy(word_model[index].name, ptr);
		if (index != 12) {
			word_model[index].start_state_num = get_start_state(ptr);
			word_model[index].end_state_num = get_end_state(ptr);
		}
		else {
			word_model[index].start_state_num = 114; // Exception for two Zero models
			word_model[index].end_state_num = 127;
		}
		ptr = strtok(NULL, " ");
		int phone_num = 0;
		// 자른 문자열이 나오지 않을 때까지 반복
		do {
			word_model[index].hmm_index[phone_num] = find_phone(ptr);
			word_model[index].hmm[phone_num] = phones[find_phone(ptr)];
			word_model[index].hmm_num = phone_num + 1;
			phone_num++;
			ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
		} while (ptr != NULL);
		//puts(buf[index]);  // 한줄 출력
		index++;
	}

	fclose(dict);


}

void read_probs() {
	FILE *unigram = fopen("unigram.txt", "r");
	char buf[13][30];
	int index_uni = 0;
	int model_index = -1;
	while (!feof(unigram))  // 파일의 끝이 아니라면
	{
		fgets(buf[index_uni], 30, unigram);  // 최대 80칸짜리 한줄 읽기
		if (buf[index_uni][strlen(buf[index_uni]) - 1] == '\n')
			buf[index_uni][strlen(buf[index_uni]) - 1] = '\0';
		char *ptr = strtok(buf[index_uni], " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
		if (ptr != NULL)
			model_index = find_word(ptr);
		else
			break;
		ptr = strtok(NULL, " ");
		if(ptr != NULL)
			transmatrix[0][word_model[model_index].start_state_num] = atof(ptr);
		
		index_uni++;
	}

	fclose(unigram);



	FILE *bigram = fopen("bigram.txt", "r");
	char buf2[150][40];
	int index_bi = 0;
	int start_model_index = -1;
	int end_model_index = -1;
	while (!feof(bigram))  // 파일의 끝이 아니라면
	{
		fgets(buf2[index_bi], 40, bigram);  // 최대 80칸짜리 한줄 읽기
		if (buf2[index_bi][strlen(buf2[index_bi]) - 1] == '\n')
			buf2[index_bi][strlen(buf2[index_bi]) - 1] = '\0';
		char *ptr = strtok(buf2[index_bi], "\t");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
	
		start_model_index = find_word(ptr);
		if (start_model_index < 0)
			break;
		
		ptr = strtok(NULL, "\t");
		
		end_model_index = find_word(ptr);
		
		ptr = strtok(NULL, " ");
		
		if (ptr != NULL) {
			bi_prob[start_model_index][end_model_index] = atof(ptr);
		}
		else
			break;

		index_bi++;
	}
	for (int i = 0; i < 13; i++) 
		bi_prob[i][12] = bi_prob[i][11];
	for (int i = 0; i < 13; i++)
		bi_prob[12][i] = bi_prob[11][i];
	float tmp = 0;
	float tmp2 = 0;
	for (int i = 0; i < 13; i++) {
		tmp = 0;
		for (int j = 0; j < 13; j++) {
			tmp += bi_prob[i][j];
		}
		
		tmp2 += tmp;
	}


	fclose(bigram);

}

void apply_transmatrix() {
	int start_state = -1;
	int end_state = -1;
	hmmType hmm;
	for (int i = 0; i < 13; i++) {
		start_state = word_model[i].start_state_num;
		end_state = word_model[i].end_state_num;
		for (int j = 0; j < word_model[i].hmm_num; j++) {
			hmm = word_model[i].hmm[j];
			for (int k = 0; k < 3; k++) { // Vertical 
				for (int l = 0; l < 4; l++) { // Horizontal
					if (strcmp(hmm.name, "sp") != 0) {
						if ((strcmp(hmm.name, "sil") != 0) && (strcmp(word_model[i].hmm[j + 1].name, "sp") == 0) && (k == 2) && (l == 3)) {
							for (int m = 0; m < 13; m++)
								transmatrix[start_state + 3 * j + k][word_model[m].start_state_num] = bi_prob[i][m] * word_model[i].hmm[j + 1].tp[0][2] * hmm.tp[k + 1][l + 1];
							transmatrix[start_state + 3 * j + k][start_state + 3 * j + l] = word_model[i].hmm[j + 1].tp[0][1] * hmm.tp[k + 1][l + 1];
						}
						else if (strcmp(hmm.name, "sil") == 0) {
							if ((k == 2) && (l == 3)) {
								for (int m = 0; m < 13; m++)
									transmatrix[start_state + 3 * j + k][word_model[m].start_state_num] = bi_prob[i][m] * hmm.tp[k + 1][l + 1];
							}
							else if (k == 0) {
								transmatrix[start_state + 3 * j + k][start_state + 3 * j + k] = hmm.tp[k + 1][k + 1];
								transmatrix[start_state + 3 * j + k][start_state + 3 * j + k + 1] = hmm.tp[k + 1][k + 2];
								transmatrix[start_state + 3 * j + k][start_state + 3 * j + k + 2] = hmm.tp[k + 1][k + 3];
								break;
							}
							else if (k == 1) {
								transmatrix[start_state + 3 * j + k][start_state + 3 * j + l] = hmm.tp[k + 1][l + 1];
							}
							else if (k == 2) {
								transmatrix[start_state + 3 * j + k][start_state + 3 * j + l] = hmm.tp[k + 1][l + 1];
							}
						}
						else
							transmatrix[start_state + 3 * j + k][start_state + 3 * j + l] = hmm.tp[k + 1][l + 1];
					}
					else {
						
						transmatrix[start_state + 3 * j + k][start_state + 3 * j + k] = hmm.tp[1][1];
						for (int m = 0; m < 13; m++)
							transmatrix[start_state + 3 * j + k][word_model[m].start_state_num] = bi_prob[i][m] * hmm.tp[1][2];
						k = 2;
						break;
					}
				}


			}
		}
	}
}


void viterbi() {
	long double GMMProb = 0;
	pdfType pdf;
	long double max_value = -INFINITY;
	hmmType phone;
	/*********Initialize*********/
	for (int i = 0; i < data_size; i++) {
		for (int j = 0; j < 127; j++)
			state_prob[i][j] = -INFINITY;
	}

	for (int i = 0; i < data_size; i++) {
		/*if (i == 0) {
			for (int j = 0; j < 127; j++) {
				state_prob[i][j] = transmatrix[0][j];
			}
			i++;
		}*/
		max_value = -INFINITY;
		
		for (int j = 1; j < 127; j++) {
			//phone = phones[
			float max_prob = -INFINITY;
			
			for (int m = 0; m < N_PDF; m++) {
				GMMProb = 0;
				pdf = phones[word_model[find_model(j)].hmm_index[(j - word_model[find_model(j)].start_state_num) / 3]].state[(j - word_model[find_model(j)].start_state_num) % 3].pdf[m];
				for (int l = 0; l < N_DIMENSION; l++) {
					//GMMProb *= (long double)exp(((long double)input[i][l] - (long double)pdf.mean[l]) * ((long double)input[i][l] - (long double)pdf.mean[l]) / ((long double)pdf.var[l] * ((long double)(-2)))) / (long double)sqrt(((long double)2 * (long double)M_PI * (long double)pdf.var[l]));
					GMMProb += log(2 * M_PI) / ((float)(-2)) + log(pdf.var[l]) / ((float)(-2)) + (input[i][l] - pdf.mean[l]) * (input[i][l] - pdf.mean[l]) / (pdf.var[l] * ((float)(-2)));
					
				}
				GMMProb += log(pdf.weight);
				if (GMMProb > max_prob) {
					max_prob = GMMProb;
				}
			}
			if (i == 0) {
				if (transmatrix[0][j] != 0) {
					state_prob[i][j] = log(transmatrix[0][j]) + (max_prob);
				}
				else
					state_prob[i][j] = -INFINITY;
			}
			else {
				max_value = -INFINITY;
				for (int k = 1; k < 127; k++) {
					if (transmatrix[k][j] == 0) {
						continue;
					}
					else {
						float tmp = log(transmatrix[k][j]) + state_prob[i - 1][k] + (max_prob); // aij * d * b(emission prob for j)
						
						if (tmp > max_value) {
							state_prob[i][j] = tmp;
							max_state[i][j] = k;
							max_value = tmp;
						}
					}
				}
				if (max_state[i][j] == 0) {
					max_state[i][j] = NULL;
					state_prob[i][j] = -INFINITY;
				}
				

			}

		}
	}
	max_value = -INFINITY;
	for (int i = 0; i < strlen(backprob_state); i++)
		backprob_state[i] = 0;
	for (int i = 1; i < 127; i++) {
		if (state_prob[data_size - 1][i] > max_value) {
			backprob_state[data_size - 1] = i;
			max_value = state_prob[data_size - 1][i];
		}
	}

	
	for (int i = data_size - 2; i >= 0; i--) {
		backprob_state[i] = max_state[i+1][backprob_state[i + 1]];
		
	}
	
}

int main() {

	make_word_model();
	read_probs();
	apply_transmatrix();
	make_word_model();
	FILE *fp = fopen("reference.txt", "r");
	FILE *Result = fopen("Result.txt", "w");
	char buf[2000][50];
	int index = 0;
	char path[120][200];
	int file_num = 0;
	fprintf(Result, "%s\n", "#!MLF!#");
	while (!feof(fp))  // 파일의 끝이 아니라면
	{

		fgets(buf[index], 50, fp);  // 최대 80칸짜리 한줄 읽기

		if (buf[index][0] != '\"')
			continue;

		if (buf[index][strlen(buf[index]) - 1] == '\n')
			buf[index][strlen(buf[index]) - 1] = '\0';

		char *ptr = strtok(buf[index], "\"");
		char *tmp = strtok(buf[index], "\"");
		char *tmp2 = strtok(tmp, ".");
		char *tmp3 = strcat(tmp2, ".rec");
		char fname[100] = "";
		sprintf(fname, "%s%s%s", "\"", tmp3, "\"");
		fprintf(Result, "%s\n", fname);
		char *ptr2 = strcat(strtok(ptr, "."), ".txt");
		//*ptr2 
		printf("%s", ptr2);

		FILE *fp2;
		fopen_s(&fp2, ptr2, "rt");

		char buf2[1000][1000];
		int index2 = 0;

		fgets(buf2[index2], 1000, fp2);  // 최대 80칸짜리 한줄 읽기
		if (buf2[index2][strlen(buf[index2]) - 1] == '\n')
			buf2[index2][strlen(buf[index2]) - 1] = '\0';
		char *ptr3 = strtok(buf2[index2], " ");
		data_size = atoi(ptr3);

		while (!feof(fp2))  // 파일의 끝이 아니라면
		{

			fgets(buf2[index2 + 1], 1000, fp2);  // 최대 80칸짜리 한줄 읽기
			if (buf2[index2 + 1][strlen(buf[index2 + 1]) - 1] == '\n')
				buf2[index2 + 1][strlen(buf[index2 + 1]) - 1] = '\0';
			sscanf(buf2[index2 + 1], "%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e", &input[index2][0], &input[index2][1], &input[index2][2], &input[index2][3], &input[index2][4], &input[index2][5], &input[index2][6], &input[index2][7], &input[index2][8], &input[index2][9], &input[index2][10], &input[index2][11], &input[index2][12], &input[index2][13], &input[index2][14], &input[index2][15], &input[index2][16], &input[index2][17], &input[index2][18], &input[index2][19], &input[index2][20], &input[index2][21], &input[index2][22], &input[index2][23], &input[index2][24], &input[index2][25], &input[index2][26], &input[index2][27], &input[index2][28], &input[index2][29], &input[index2][30], &input[index2][31], &input[index2][32], &input[index2][33], &input[index2][34], &input[index2][35], &input[index2][36], &input[index2][37], &input[index2][38]);
			

			index2++;
		}

		fclose(fp2);

		//Write Code Here

		viterbi();
		char *arr = "aaaaaaaa";
		int cur_state = 0;
		int num_words = 0;
		int words[20] = { 0, };
		for (int i = 0; i < 20; i++)
			words[i] = 0;
		for (int i = 0; i < data_size; i++) {
			if (i == 0) {
				cur_state = find_model(backprob_state[i]);
				num_words += 1;
				words[num_words - 1] = cur_state;
			}
			else if ((backprob_state[i] == word_model[cur_state].end_state_num) && (cur_state != -1)) {
				//printf("%s\n", word_model[cur_state].name);
				cur_state = -1;
			}
			else if ((i != data_size - 1) && (cur_state != -1) && (backprob_state[i] == word_model[cur_state].end_state_num - 1) && (backprob_state[i + 1] != word_model[cur_state].end_state_num)) {
				//printf("%s\n", word_model[cur_state].name);
				cur_state = -1;
			}
			else if ((cur_state != find_model(backprob_state[i])) && (word_model[find_model(backprob_state[i])].start_state_num == backprob_state[i])) {
				cur_state = find_model(backprob_state[i]);
				num_words += 1;
				words[num_words - 1] = cur_state;
				//printf("%s\n", word_model[cur_state].name);
			}

		}

		int num_ans = 0;
		int silcount = 0;
		for (int j = 1; j < num_words; j++) {
			 if (strcmp(word_model[words[j]].name, "<s>") == 0) {
				if (silcount == 0) {
					if (num_ans == 7) {
						
						break;
					}
					silcount++;
					continue;
				}
				else if (silcount == 1) {
					if (num_ans == 7) {
						
						break;
					}
					else continue;
				}
			}
			else if (num_ans == 7) {
				
				break;
			}
			else {
				fprintf(Result, "%s\n", word_model[words[j]].name);
				num_ans++;
			}
		}
		fprintf(Result, "%s", ".\n");


		index++;

	}
	fclose(fp);
	fclose(Result);

	

	system("pause");

	return 0;
}