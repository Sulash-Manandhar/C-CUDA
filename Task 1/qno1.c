#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct matrix
{
  int row;
  int col;
  double *value;
};

struct matrix_pairs
{
  // int capacity;
  struct matrix *matrix;
};

struct thread_data
{
  int thread_id;
  int slice_value;
  int start;
  int end;
  struct matrix_pairs *matrix_pairs;
};

double *allocate_array(int size)
{
  double *array = (double *)malloc(sizeof(double) * size);
  return array;
}

int getIndex(int row, int col, int n_col)
{
  return row * n_col + col;
}

void printMatrix(struct matrix M)
{
  printf("%d,%d\n", M.row, M.col);

  for (int i = 0; i < M.row; i++)
  {
    for (int j = 0; j < M.col; j++)
    {
      printf("%lf ", M.value[getIndex(i, j, M.col)]);
    }
    printf("\n");
  }
  printf("\n");
}


pthread_mutex_t mutex;


  char *output_filename = "matrixresults2050200.txt";

void write_matrix_output(struct matrix M)
{
  FILE *fp=NULL;
  pthread_mutex_lock(&mutex);

  fp = fopen(output_filename, "a");
  fprintf(fp, "%d,%d\n", M.row, M.col);

  for (int i = 0; i < M.row; i++)
  {
    for (int j = 0; j < M.col; j++)
    {
      fprintf(fp, "%lf ", M.value[getIndex(i, j, M.col)]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "\n");

  fclose(fp);
  

  pthread_mutex_unlock(&mutex);
}

int unsucc_matrix_multiplications = 0;

void perform_multiplication(struct matrix_pairs *matrix_pairs)
{
  struct matrix A = matrix_pairs->matrix[0];
  struct matrix B = matrix_pairs->matrix[1];

  if (A.col == B.row)
  {
    struct matrix C;

    C.row = A.row;
    C.col = B.col;
    C.value = allocate_array(C.row * C.col);

    for (int i = 0; i < C.row; i++)
    {
      for (int j = 0; j < C.col; j++)
      {
        C.value[getIndex(i, j, C.col)] = 0.0;
        for (int k = 0; k < B.row; k++)
        {
          C.value[getIndex(i, j, C.col)] += A.value[getIndex(i, k, A.col)] * B.value[getIndex(k, j, B.col)];
        }
        // printf("%f ", C.value[getIndex(i, j, C.col)]);
      }
 
    }
    write_matrix_output(C);
  }
  else
  {
    printf("Matrix Multiplication unsuccessful...\n");
    unsucc_matrix_multiplications++;
    return;
  }
}

//
void *perform_multiplications_parallely(void *arg)
{
  struct thread_data *data = (struct thread_data *)arg;
  int id = data->thread_id;
  int slice_value = data->slice_value;
  int start = data->start;
  int end = data->end;
  struct matrix_pairs *matrix_pairs = data->matrix_pairs;


  printf("Thread %d initiated with %d matrix multiplications to be carried out...\n", id, slice_value);

  if (slice_value > 0)
  {
    for (int i = start; i <= end; i++)
    {
      

      perform_multiplication(&matrix_pairs[i]);
    }
    return NULL;
  }
  printf("Thread %d had no work to do...\n", id);
  return NULL;
}


int count_total_multiplications(char *files[], int n)
{
  int count = 0;

  for (int i = 0; i < n; i++)
  {
    FILE *fp = fopen(files[i], "r");
    if (fp == NULL)
    {
      printf("Error opening file %s\n", files[i]);
      continue;
    }
    else
    {
      printf("File %s opened successfully\n", files[i]);

      fp = fopen(files[i], "r");

      while (!feof(fp))
      {
        for (int i = 0; i < 2; i++)
        {
          int m, n;
          fscanf(fp, "%d,%d", &m, &n);

          double temp_val = 0.0;

          for (int i = 0; i < m; i++)
          {
            for (int j = 0; j < n - 1; j++)
            {
              fscanf(fp, "%lf,", &temp_val);
            }
            fscanf(fp, "%lf\n", &temp_val);
          }
        }
        count++;
      }
    }

    fclose(fp);
  }
  return count;
}
void add_matrix_pairs(struct matrix_pairs *matrix_pair_list, char *files[], int n)
{
  // int matrix_pairs_count = 0;
  int matrix_pair_index = 0;

  for (int i = 0; i < n; i++)
  {
    FILE *fp = fopen(files[i], "r");
    if (fp == NULL)
    {
    	continue;
    }
    else
    {
      fp = fopen(files[i], "r");

      while (!feof(fp))
      {
        struct matrix_pairs *matrix_pair = (struct matrix_pairs *)malloc(sizeof(struct matrix_pairs));

        matrix_pair->matrix = (struct matrix *)malloc(sizeof(struct matrix) * 2);

        for (int i = 0; i < 2; i++)
        {
          struct matrix *M;
          M = &matrix_pair->matrix[i];
          fscanf(fp, "%d,%d", &M->row, &M->col);
         
          M->value = allocate_array(M->row * M->col);

          int n = 0;
          double temp_val = 0.0;

          for (int i = 0; i < M->row; i++)
          {
    
            for (int j = 0; j < M->col - 1; j++)
            {
              fscanf(fp, "%lf,", &temp_val);
             
              M->value[getIndex(i, j, M->col)] = temp_val;
            }
            fscanf(fp, "%lf\n", &temp_val);
            M->value[getIndex(i, M->col - 1, M->col)] = temp_val;
          }
        }
        matrix_pair_list[matrix_pair_index] = *matrix_pair;
        matrix_pair_index++;
        free(matrix_pair);
      }
    }
  }
}

int main()
{
  int threadNum;

  printf("Enter number of threads: ");
  scanf("%d", &threadNum);
  while (threadNum < 1 || threadNum >= 1000)
  {
    printf("Invalid number of threads...\n");
    printf("Enter number of threads...");
    scanf("%d", &threadNum);
    return -1;
  }

  printf("\n");

  int file_count= 5;

  char *files[] = {
        "SampleMatricesWithErrors.txt"
       "SampleMatricesWithErrors.txt",
       "SampleMatricesWithErrors2.txt",
       "SampleMatricesWithErrors3.txt",
       "SampleMatricesWithErrors4.txt",
       "SampleMatricesWithErrors5.txt"
  };

  int total_multiplications_size = count_total_multiplications(files, file_count);
  printf("\nTotal number of matrix multiplications is: %d\n\n", total_multiplications_size);

 
  struct matrix_pairs *matrix_pair_list = (struct matrix_pairs *)malloc(sizeof(struct matrix_pairs) * total_multiplications_size);


  add_matrix_pairs(matrix_pair_list, files, file_count);

  int *sliceList = (int *)malloc(sizeof(int) * threadNum);
  int remainder_values = total_multiplications_size % threadNum;

  for (int i = 0; i < threadNum; i++)
  {
    sliceList[i] = total_multiplications_size / threadNum;
  }

  for (int i = 0; i < remainder_values; i++)
  {
    sliceList[i] = sliceList[i] + 1;
  }

  int *startList = (int *)malloc(sizeof(int) * threadNum);
  int *endList = (int *)malloc(sizeof(int) * threadNum);

  for (int i = 0; i < threadNum; i++)
  {
    if (i == 0)
    {
      startList[i] = 0;
    }
    else
    {
      startList[i] = endList[i - 1] + 1;
    }
    endList[i] = startList[i] + sliceList[i] - 1;
  }

   struct thread_data *mainStruct = (struct thread_data *)malloc(sizeof(struct thread_data) * threadNum);

   for (int i = 0; i < threadNum; i++)
   {
     mainStruct[i].thread_id = i;
     mainStruct[i].slice_value= sliceList[i];
     mainStruct[i].start = startList[i];
     mainStruct[i].end = endList[i];
     mainStruct[i].matrix_pairs = matrix_pair_list;

   }

   pthread_t threads[threadNum];


	pthread_mutex_init(&mutex, NULL);

	printf("Executing %d threads simultaneously...\n\n", threadNum);

   for (int i = 0; i < threadNum; i++)
   {
     pthread_attr_t attr;
     pthread_attr_init(&attr);

     pthread_create(&threads[i], &attr, perform_multiplications_parallely, (void *)&mainStruct[i]);
   }
   for (int i = 0; i < threadNum; i++)
   {
     pthread_join(threads[i], NULL);
   }

   int succ_matrix_multiplications= total_multiplications_size-unsucc_matrix_multiplications;

  printf("\nThe number of Successfull matrix multiplications is: %d\n", succ_matrix_multiplications);
  printf("The number of Unsuccessfull matrix multiplications is: %d\n", unsucc_matrix_multiplications);

  printf("\nThe final output matrices are stored in the file named \"%s\"...\n", output_filename);


pthread_mutex_destroy(&mutex);

  
  for (int i = 0; i < total_multiplications_size; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      free(matrix_pair_list[i].matrix[j].value);
    }
    free(matrix_pair_list[i].matrix);
  }
  free(matrix_pair_list);

  free(sliceList);
  free(startList);
	free(endList);
	free(mainStruct);


  return 0;
}
