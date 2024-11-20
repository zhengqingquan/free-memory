#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 这个要求每个元素都是单独申请的内容，
// 这个是释放结构体数组的，因为使用了->、.等指针运算符
// 这里的数组共享同一块内存
#define RELEASE_ARRAY_START(SturetType,SturetType2, ArrayPtr, ArraySize) \
    do \
    { \
        printf("ZQQ1 debug ptr: 0x%p\n", *ptr);\
        \
        void** backup_array_ptr = (void**)&((*ptr)->ArrayPtr);\
        printf("ZQQ2 debug backup_array_ptr: 0x%p\n", *backup_array_ptr);\
        \
        int* backup_array_size = (int*)&((*ptr)->ArraySize);\
        printf("ZQQ3 debug backup_array_size: %d\n", *backup_array_size);\
        \
        SturetType* temp_ptr = (SturetType*)*backup_array_ptr;\
        printf("ZQQ4 debug temp_ptr: 0x%p\n", temp_ptr);\
        \
        SturetType2* SturetType2_ptr = (SturetType2*)(*ptr)->ArrayPtr;\
        printf("ZQQ5 debug SturetType2_ptr: 0x%p\n", &(SturetType2_ptr[2]));\
        \
        printf("size: %d\n", *backup_array_size);\
        for (size_t i = 0; i < *backup_array_size; i++)\
        {\
            printf("backup_array_size[%d]: 0x%p\n",i, &SturetType2_ptr[i]);\
            printf("backup_array_size[%d]: %s\n",i, SturetType2_ptr[i].name);\
            /*printf("backup_array_size[%d]: 0x%p\n",i, &((SturetType*)*ptr)->ArrayPtr[i]);*/\
            /*printf("backup_array_size[%d]: %s\n",i, ((SturetType*)*ptr)->ArrayPtr[i].name);*/\
        }\
        for (size_t i = 0; i < *backup_array_size; i++)\
        {\
            /*void** ptr = (void**)&(SturetType2_ptr[2]);*/\
            SturetType2* ptr = (SturetType2*)&(SturetType2_ptr[i]);\
            /*SturetType2* ptr = (SturetType2*)&temp;*/\
            /*SturetType2* ptr = &((SturetType2*)SturetType2_ptr)[i];*/\
            printf("ptr[%d]: 0x%p\n",i, &ptr);\
            printf("ptr[%d]: %s\n",i, &ptr->name);
        
                    // void** ptr = (void**)&StructPtr;
                    // void** ptr = (void**)&((SturetType*)backup_array_ptr[i]);
        /*
        void** backup_array_ptr = (void**)&ArrayPtr; \
        int* backup_array_size = (int*)&ArraySize; \
        SturetType* StructPtr = (SturetType*)ArrayPtr;\
        printf("zqq StructPtr 0x%p, ArraySize: %d\n", StructPtr, StructPtr->ArraySize); \
        for (size_t i = 0; i < StructPtr->ArraySize; i++)\
        {
            */

            // RELEASE_MEMORY(((SturetType*)*ptr)->FiledName);
            // void* item = &company.departments[i];
        /*
        void** temp = ArrayPtrHead;
        void** ArrayPtrHead = (void**)&StructPtr; 
        */
        /*
        RELEASE_ARRAY(ArrayPtr, ArraySize);
        releaseArray((void***)&(ArrayPtr), &(ArraySize))*/

/*
#define RELEASE_ARRAY_END                     \
        }\
        RELEASE_ARRAY(*backup_array_ptr, *backup_array_size); \
    }                                                     \
    while (0);
*/

// 释放数组共享内存
#define RELEASE_ARRAY_END                     \
        }\
        RELEASE_MEMORY(*backup_array_ptr);\
    }                                                     \
    while (0);

// TODO 这里的void** ptr可以优化，设置为SturetType** ptr = (SturetType**)(*ptr)->ArrayPtr;
#define RELEASE_STRUCT_START(SturetType, StructPtr)         \
    do                                                      \
    {                                                       \
        if (StructPtr == NULL)                              \
        {                                                   \
            printf("StructPtr 0x%p is null.\n", StructPtr); \
            break;                                          \
        }                                                   \
        printf("debug zqq %p\n", StructPtr);\
        /*void** ptr = (void**)&StructPtr;*/ \
        SturetType** ptr = (SturetType**)&StructPtr;

#define RELEASE_STRUCT_END \
        RELEASE_MEMORY(*ptr);\
    } while (0);

// TODO 这里的SturetType可以优化，不需要SturetType传参
// 用于释放结构体中的指针。因为使用->运算符，不能用于普通数组。
#define RELEASE_STRUCT_MEMORY(SturetType, FiledName) \
    RELEASE_MEMORY(((SturetType*)*ptr)->FiledName);

// 用于释放结构体中的数组成员中的每一个指针。因为使用了->运算符，不能用于普通数组。
#define RELEASE_STRUCT_ARRAY(SturetType, ArrayFiledName, ArrayFiledSize) \
    RELEASE_ARRAY(((SturetType*)*ptr)->ArrayFiledName, ((SturetType*)*ptr)->ArrayFiledSize);

// 用于释放普通指针。
#define RELEASE_MEMORY(MemoryPtr) \
    releaseMemory((void**)&(MemoryPtr))

// 用于释放数组中的每一个指针。
#define RELEASE_ARRAY(ArrayPtr, ArraySize) \
    releaseArray((void***)&(ArrayPtr), &(ArraySize))

// 可以保留，因为释放内存不看指针类型。
void releaseMemory(void** ptr)
{
    if (ptr == NULL)
    {
        printf("pointer 0x%p is null.\n", ptr);
        return;
    }

    if (*ptr == NULL)
    {
        printf("freeing 0x%p (null pointer).\n", *ptr);
        return;
    }

    printf("freeing 0x%p\n", *ptr);
    free(*ptr);
    *ptr = NULL;
}

// 会单独释放数组中的每个元素，最后释放数组，将数组大小设置为0。
void releaseArray(void*** ptrArray, int* size)
{
    if (ptrArray == NULL)
    {
        printf("ptrArray 0x%p is null.\n", ptrArray);
        return;
    }

    if (*ptrArray == NULL)
    {
        printf("freeing 0x%p (null pointer).\n", *ptrArray);
        return;
    }

    // 逐个释放每个字符串
    printf("ptrArray 0x%p, size: %d\n",*ptrArray, *size);
    for (int i = 0; i < *size; i++)
    {
        RELEASE_MEMORY((*ptrArray)[i]);
    }

    // 释放字符串数组本身
    printf("freeing ptrArray 0x%p\n", *ptrArray);
    free(*ptrArray);
    *ptrArray = NULL;
    *size = 0;
}

// 不用单独释放数组中的每个元素，
void releaseArray_not_each(void*** ptrArray, int* size)
{

}

typedef struct
{
    int id; // 教师ID
    int sex; // 教师性别
    char* name; // 教师姓名
    char** course_names; // 教师的课程的名字数组
    int course_num; // 教师的课程的数量
} teachers;

typedef struct
{
    int id; // 学生ID
    char* name; // 学生姓名
    float score; // 学生成绩
    char** dog_names; // 学生的狗的名字数组
    int dog_num; // 学生的狗的数量
    teachers* teacher; // 学生的老师
    int teacher_num; // 学生的老师的数量
} Student;

Student* create_student(void)
{
    Student* student = (Student*)malloc(sizeof(Student)); // 分配学生结构体的内存
    if (student == NULL)
    {
        printf("malloc failed!\n");
        return NULL;
    }
    printf("allocated memory for student: 0x%p\n", student);

    student->name = (char*)malloc(50 * sizeof(char)); // 分配学生姓名的内存
    if (student->name == NULL)
    {
        printf("malloc failed!\n");
        free(student);
        return NULL;
    }
    strcpy(student->name, "Alice"); // 初始化学生姓名
    printf("allocated memory for student name: %s, 0x%p\n", student->name, student->name);

    student->dog_num = 4; // 初始化学生的狗的数量
    printf("student->dog_num: %d\n", student->dog_num);
    student->dog_names = (char**)malloc(student->dog_num * sizeof(char*)); // 分配学生的狗的名字数组的内存
    printf("allocated memory for student dog names array: 0x%p\n", student->dog_names);
    for (size_t i = 0; i < student->dog_num; i++)
    {
        student->dog_names[i] = (char*)malloc(50 * sizeof(char)); // 分配每个狗的名字的内存
        snprintf(student->dog_names[i], 50, "dog%d", i); // 初始化每个狗的名字
    }
    for (size_t i = 0; i < student->dog_num; i++)
    {
        printf("dog%d: %s, 0x%p\n", i, student->dog_names[i], student->dog_names[i]);
    }

    student->teacher_num = 4; // 初始化学生的老师的数量
    printf("student->teacher_num: %d\n", student->teacher_num);
    student->teacher = (teachers*)malloc(student->teacher_num * sizeof(teachers)); // 分配学生的老师的结构体数组的内存
    printf("allocated memory for student teacher array: 0x%p\n", student->teacher);
    for (size_t i = 0; i < student->teacher_num; i++)
    {
        printf("------ teacher%d 0x%p ------\n", i, &student->teacher[i]);
        student->teacher[i].id = i + 1; // 初始化每个老师的ID
        student->teacher[i].name = (char*)malloc(50 * sizeof(char)); // 分配每个老师的姓名的内存
        snprintf(student->teacher[i].name, 50, "teacher%d", i); // 初始化每个老师的姓名
        printf("allocated memory for teacher%d name: 0x%p\n", i, student->teacher[i].name);

        student->teacher[i].course_num = 3; // 初始化每个老师的课程的数量
        printf("teacher%d->course_num: %d\n", i, student->teacher[i].course_num);
        student->teacher[i].course_names = (char**)malloc((student->teacher[i].course_num) * sizeof(char*)); // 分配每个老师的课程的名字数组的内存
        printf("allocated memory for teacher%d course names array: 0x%p\n", i, student->teacher[i].course_names);
        for (size_t j = 0; j < student->teacher[i].course_num; j++)
        {
            student->teacher[i].course_names[j] = (char*)malloc(50 * sizeof(char)); // 分配每个课程的名字的内存
            snprintf(student->teacher[i].course_names[j], 50, "course%d-%d", i, j); // 初始化每个课程的名字
            printf ("allocated memory for course%d-%d name: 0x%p\n", i, j, student->teacher[i].course_names[j]);
        }
    }

    printf("------ student malloc end ------\n");
    return student;
}

int main()
{
    Student* student = create_student();

    RELEASE_STRUCT_START(Student, student) // 释放学生结构体的内存
        printf("------ student ------\n");
        printf("student name: 0x%p\n", student->name);
        RELEASE_STRUCT_MEMORY(Student, name) // 释放学生ID的内存
        printf("student name: 0x%p\n", student->name);
        printf("------ student ------\n");

        printf("------ dogs ------\n");
        printf("dog_num: %d\n", student->dog_num);
        printf("dog_names: 0x%p\n", student->dog_names);
        RELEASE_STRUCT_ARRAY(Student, dog_names, dog_num); // 释放学生的狗的名字数组的内存
        printf("dog_num: %d\n", student->dog_num);
        printf("dog_names: 0x%p\n", student->dog_names);
        printf("------ dogs ------\n");

        printf("------ teacher ------\n");
        printf("teacher: 0x%p\n", student->teacher);
        RELEASE_ARRAY_START(Student,teachers, teacher, teacher_num)
            // RELEASE_STRUCT_ARRAY();
            // RELEASE_ARRAY_START(teachers,char*, course_names, course_num)
            // RELEASE_ARRAY_END
        RELEASE_ARRAY_END
        printf("teacher: 0x%p\n", student->teacher);
        printf("------ teacher ------\n");

    RELEASE_STRUCT_END;

    printf("allocated memory for student: 0x%p\n", student);

    return 0;

    // --------------------- 示例1 ----------

    int size = 4; // 字符串数组的大小
    char** strArray = (char**)malloc(size * sizeof(char*)); // 分配字符串数组的内存

    // 检查内存分配是否成功
    if (strArray == NULL)
    {
        printf("malloc failed!\n");
        return 1;
    }

    // 为每个字符串分配内存
    for (int i = 0; i < size; i++)
    {
        strArray[i] = (char*)malloc(50 * sizeof(char)); // 假设每个字符串最大长度为49（1个字符用于空字符结束）

        // 检查内存分配是否成功
        if (strArray[i] == NULL)
        {
            printf("内存分配失败！\n");

            // 释放已分配的内存
            for (int j = 0; j < i; j++)
            {
                free(strArray[j]);
            }
            free(strArray);
            return 1;
        }
    }

    // 初始化字符串
    strcpy(strArray[0], "Hello, world!");
    strcpy(strArray[1], "Welcome to C programming.");
    strcpy(strArray[2], "This is a dynamically allocated string array.");
    strcpy(strArray[3], "Enjoy coding!");

    // 打印字符串数组内容
    printf("字符串数组内容：\n");
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", strArray[i]);
    }

    RELEASE_ARRAY(strArray, size); // 释放字符串数组的内存
    printf("strArray 0x%p is released.\n", strArray);
    printf("size of strArray: %d\n", size);

    return 0;
}
