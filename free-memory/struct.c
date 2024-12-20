#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * -----------------------------------------------
 * |                 内存释放                     |
 * -----------------------------------------------
 */

/**
 * @brief 用于释放内存，并将指针设置为NULL。
 */
#define RELEASE_MEMORY(MemoryPtr)\
    releaseMemory((void**)&(MemoryPtr));

/**
 * @brief 用于释放动态二维数组。会单独释放数组中的每个元素，最后释放数组，并将数组大小设置为 0。
 */
#define RELEASE_PTR_ARRAY(ArrayPtr, ArraySize)\
    releasePtrArray((void***)&(ArrayPtr), &(ArraySize));

/**
 * @brief 用于释放对象数组。不会释放数组中的每个元素，只会释放数组本身，并将数组大小设置为 0。
 */
#define RELEASE_OBJECT_ARRAY(ArrayPtr, ArraySize)\
    releaseObjectArray((void**)&(ArrayPtr), &(ArraySize));

/**
 * @brief 释放内存，并将指针设置为NULL。
 * 
 * @param ptr 指向需要释放的内存的指针。
 * @return int  1 表示成功，0 表示失败。
 */
int releaseMemory(void** ptr)
{
    if (ptr == NULL)
    {
        printf("pointer 0x%p is null.\n", ptr);
        return 0;
    }

    if (*ptr == NULL)
    {
        printf("freeing 0x%p (null pointer).\n", ptr);
        return 0;
    }

    printf("freeing 0x%p\n", *ptr);
    free(*ptr);
    *ptr = NULL;

    return 1;
}

/**
 * @brief 用于释放动态二维数组。会单独释放数组中的每个元素，最后释放数组，并将数组大小设置为 0。
 * 
 * @param ptrArray 指向需要释放的数组的指针。
 * @param size 指向数组大小的指针。
 */
void releasePtrArray(void*** ptrArray, int* size)
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

/**
 * @brief 用于释放存储对象的数组。不会释放数组中的每个元素，只会释放数组本身，并将数组大小设置为 0。
 * 
 * @param ptrArray 指向需要释放的数组的指针。
 * @param size 指向数组大小的指针。
 */
void releaseObjectArray(void** ptrArray, int* size)
{
    if (releaseMemory(ptrArray) == 0)
    {
        return;
    }

    *size = 0;
}

/**
 * -----------------------------------------------
 * |                 结构体释放                   |
 * -----------------------------------------------
 */

/**
 * @brief 用于释放结构体内存的宏定义。表示释放结构体开始。
 * 
 * 与宏 RELEASE_STRUCT_END 配合使用。
 */
#define RELEASE_STRUCT_START(SturetType, StructPtr)\
    do\
    {\
        int nested_level = 0;\
        if (StructPtr == NULL)\
        {\
            printf("[nested level %d] struct variable '%s'(0x%p) is null.\n", nested_level, #StructPtr, &StructPtr);\
            break;\
        }\
        printf("[nested level %d] struct variable '%s': 0x%p\n", nested_level, #StructPtr, StructPtr);\
        SturetType** ptr = (SturetType**)&StructPtr;

/**
 * @brief 用于释放结构体内存的宏定义。表示释放结构体结束。
 * 
 * 与宏 RELEASE_STRUCT_START 配合使用。
 */
#define RELEASE_STRUCT_END\
        RELEASE_MEMORY(*ptr);\
        printf("[nested level %d] release struct end\n", nested_level);\
    } while (0);

/**
 * @brief 用于释放结构体对象数组成员内存的宏定义。
 * 
 * 与 RELEASE_STRUCT_OBJECT_ARRAY_END 配合使用。
 */
#define RELEASE_STRUCT_OBJECT_ARRAY_START(SturetType, ArrayPtr, ArraySize)\
    do\
    {\
        nested_level++;\
        \
        void** array_ptr = (void**)&((*ptr)->ArrayPtr);\
        printf("[nested level %d] array variable '%s': 0x%p\n", nested_level, #ArrayPtr, *array_ptr);\
        \
        SturetType* backup_array_ptr = (SturetType*)(*array_ptr);\
        \
        int* array_size_ptr = (int*)&((*ptr)->ArraySize);\
        printf("[nested level %d] array size '%s': %d\n", nested_level, #ArraySize, *array_size_ptr);\
        \
        for (size_t i = 0; i < *array_size_ptr; i++)\
        {\
            SturetType* temp = (SturetType*)&backup_array_ptr[i];\
            SturetType** ptr = (SturetType**)&temp;\
            printf("[nested level %d] %s[%d]: 0x%p\n", nested_level, #ArrayPtr, i, *ptr);

/**
 * @brief 用于释放结构体对象数组成员内存的宏定义。
 * 
 * 与 RELEASE_STRUCT_OBJECT_ARRAY_START 配合使用。
 */
#define RELEASE_STRUCT_OBJECT_ARRAY_END\
        }\
        RELEASE_OBJECT_ARRAY(*array_ptr, *array_size_ptr);\
        \
        printf("[nested level %d] release struct end\n", nested_level);\
        nested_level--;\
    }\
    while (0);

/**
 * @brief 用于释放结构体二维指针数组成员内存的宏定义。
 * 
 * 与 RELEASE_STRUCT_PTR_ARRAY_END 配合使用。
 */
#define RELEASE_STRUCT_PTR_ARRAY_START(SturetType, ArrayPtr, ArraySize)\
    do \
    {\
        nested_level++;\
        \
        void** array_ptr = (void**)&((*ptr)->ArrayPtr);\
        printf("[nested level %d] array variable '%s': 0x%p\n", nested_level, #ArrayPtr, *array_ptr);\
        \
        SturetType** backup_array_ptr = (SturetType**)(*array_ptr);\
        \
        int* array_size_ptr = (int*)&((*ptr)->ArraySize);\
        printf("[nested level %d] array size '%s': %d\n", nested_level, #ArraySize, *array_size_ptr);\
        \
        for (size_t i = 0; i < *array_size_ptr; i++)\
        {\
            SturetType** ptr = (SturetType**)&backup_array_ptr[i];\
            printf("[nested level %d] %s[%d]: 0x%p\n", nested_level, #ArrayPtr, i, *ptr);

/**
 * @brief 用于释放结构体二维指针数组成员内存的宏定义。
 * 
 * 与 RELEASE_STRUCT_PTR_ARRAY_START 配合使用。
 */
#define RELEASE_STRUCT_PTR_ARRAY_END\
        }\
        \
        RELEASE_PTR_ARRAY(*array_ptr, *array_size_ptr);\
        \
        printf("[nested level %d] release struct end\n", nested_level);\
        nested_level--;\
    }\
    while (0);

/**
 * @brief 用于释放结构体中的指针。因为使用了 '->' 运算符，不能用于普通指针。
 */
#define RELEASE_STRUCT_MEMORY(FiledName)\
    nested_level++;\
    printf("[nested level %d] pointer variable '%s': 0x%p\n", nested_level, #FiledName, (*ptr)->FiledName);\
    RELEASE_MEMORY((*ptr)->FiledName);\
    nested_level--;

/**
 * @brief 用于释放结构体的二维指针数组成员。
 * 
 * 会释放数组中的每一个指针。因为使用了 '->' 运算符，不能用于普通数组。
 */
#define RELEASE_STRUCT_PTR_ARRAY(ArrayFiledName, ArrayFiledSize)\
    nested_level++;\
    printf("[nested level %d] pointer variable '%s': 0x%p\n", nested_level, #ArrayFiledName, (*ptr)->ArrayFiledName);\
    RELEASE_PTR_ARRAY((*ptr)->ArrayFiledName, (*ptr)->ArrayFiledSize);\
    nested_level--;

/**
 * @brief 用于释放结构体的对象数组成员。
 * 
 * 不会释放数组中的每一个元素。因为使用了 '->' 运算符，不能用于普通数组。
 */
#define RELEASE_STRUCT_OBJECT_ARRAY(ArrayFiledName, ArrayFiledSize)\
    RELEASE_OBJECT_ARRAY((*ptr)->ArrayFiledName, (*ptr)->ArrayFiledSize);

/**
 * @brief 开始嵌套结构体的宏定义。
 * 
 * 与 NESTED_STRUCT_END 配合使用。
 */
#define NESTED_TYPEDEF_STRUCT_START(SturetType, FiledName)\
    do \
    {\
        nested_level++;\
        SturetType* temp_ptr = (SturetType*)&((*ptr)->FiledName);\
        printf("[nested level %d] pointer variable '%s': 0x%p\n", nested_level, #FiledName, temp_ptr);\
        SturetType** ptr = (SturetType**)&(temp_ptr);

/**
 * @brief 开始嵌套结构体的宏定义。
 * 
 * 与 NESTED_STRUCT_END 配合使用。
 */
#define NESTED_STRUCT_START(SturetType, FiledName)\
    NESTED_TYPEDEF_STRUCT_START(struct SturetType, FiledName)

/**
 * @brief 结束嵌套结构体的宏定义。
 * 
 * 与 NESTED_STRUCT_START 或 NESTED_TYPEDEF_STRUCT_START 配合使用。
 */
#define NESTED_STRUCT_END\
        nested_level--;\
    }\
    while (0);

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
    int id; // 朋友ID
    char* pet_name; // 朋友姓名
} pet;

typedef struct
{
    int id; // 朋友ID
    char* name; // 朋友姓名
    pet** pets; // 朋友的宠物数组
    int pet_num; // 朋友的宠物数量
} friend;

typedef struct
{
    int id; // 学生ID
    char* name; // 学生姓名
    float score; // 学生成绩
    char** dog_names; // 学生的狗的名字数组
    int dog_num; // 学生的狗的数量
    teachers* teacher; // 学生的老师
    int teacher_num; // 学生的老师的数量
    friend** friends; // 学生的朋友
    int friends_num; // 学生的朋友的数量
    struct explain2
    {
        char* word_means;
        int word_means_count; // 包含的释义数量
        char** case_word; // 数组，包含例子释义
        int case_word_count;
        friend** friends; // 学生的朋友
    }explain;
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

    student->friends_num = 3; // 初始化学生的朋友的数量
    printf("student->friends_num: %d\n", student->friends_num);
    student->friends = (friend**)malloc(student->friends_num * sizeof(friend*)); // 分配学生的朋友的结构体数组的内存
    memset(student->friends, 0, student->friends_num * sizeof(friend*));
    printf("allocated memory for student friends array: 0x%p\n", student->friends);

    for (size_t i = 0; i < student->friends_num; i++)
    {
        student->friends[i] = (friend*)malloc(sizeof(friend)); // 分配每个朋友的结构体的内存
        student->friends[i]->id = i + 1; // 初始化每个朋友的ID
        printf("------ friend%d 0x%p ------\n", i, student->friends[i]);
        student->friends[i]->name = (char*)malloc(50 * sizeof(char)); // 分配每个朋友的姓名的内存
        snprintf(student->friends[i]->name, 50, "friend%d", i); // 初始化每个朋友的姓名
        printf("allocated memory for %s name: 0x%p\n", student->friends[i]->name, student->friends[i]->name);

        student->friends[i]->pet_num = 2; // 初始化每个朋友的宠物的数量
        printf("friend%d->pet_num: %d\n", i, student->friends[i]->pet_num);
        student->friends[i]->pets = (pet**)malloc(student->friends[i]->pet_num * sizeof(pet*)); // 分配每个朋友的宠物的结构体数组的内存
        printf("allocated memory for pets array: 0x%p\n", student->friends[i]->pets);
        for (size_t j = 0; j < student->friends[i]->pet_num; j++)
        {
            student->friends[i]->pets[j] = (pet*)malloc(sizeof(pet)); // 分配每个朋友的宠物的结构体的内存
            student->friends[i]->pets[j]->id = j + 1; // 初始化每个朋友的宠物的ID
            student->friends[i]->pets[j]->pet_name = (char*)malloc(50 * sizeof(char)); // 分配每个朋友的宠物的姓名的内存
            snprintf(student->friends[i]->pets[j]->pet_name, 50, "pet%d-%d", i, j); // 初始化每个朋友的宠物的姓名
            printf("------ pet%d-%d 0x%p ------\n", i, j, student->friends[i]->pets[j]);
            printf("allocated memory for %s pet name: 0x%p\n", student->friends[i]->pets[j]->pet_name, student->friends[i]->pets[j]->pet_name);
        }
    }

    student->explain.word_means = (char*)malloc(50 * sizeof(char)); // 分配学生的释义的内存
    strcpy(student->explain.word_means, "this is a word means"); // 初始化学生的释义
    printf("allocated memory for student explain: %s, 0x%p\n", student->explain.word_means, student->explain.word_means);

    student->explain.case_word_count = 2; // 初始化学生的释义的例子释义的数量
    student->explain.case_word = (char**)malloc(student->explain.case_word_count * sizeof(char*)); // 分配学生的释义的例子释义的数组的内存
    printf("allocated memory for student explain case word array: 0x%p\n", student->explain.case_word);
    for (size_t i = 0; i < student->explain.case_word_count; i++)
    {
        student->explain.case_word[i] = (char*)malloc(50 * sizeof(char)); // 分配每个例子释义的内存
        snprintf(student->explain.case_word[i], 50, "case%d", i); // 初始化每个例子释义
        printf("allocated memory for case%d word: 0x%p\n", i, student->explain.case_word[i]);
    }
    

    printf("------ student malloc end ------\n");
    return student;
}

int main()
{
    Student* student = create_student();

    printf("---------- begin free ------------\n");

    RELEASE_STRUCT_START(Student, student) // 释放学生结构体的内存

        // printf("------ student ------\n");
        // printf("student name: 0x%p\n", student->name);
        RELEASE_STRUCT_MEMORY(name) // 释放学生ID的内存
        // printf("student name: 0x%p\n", student->name);
        // printf("------ student ------\n");

        // printf("------ dogs ------\n");
        // printf("dog_num: %d\n", student->dog_num);
        // printf("dog_names: 0x%p\n", student->dog_names);
        RELEASE_STRUCT_PTR_ARRAY(dog_names, dog_num); // 释放学生的狗的名字数组的内存
        // printf("dog_num: %d\n", student->dog_num);
        // printf("dog_names: 0x%p\n", student->dog_names);
        // printf("------ dogs ------\n");

        // printf("------ teacher ------\n");
        // printf("teacher: 0x%p\n", student->teacher);
        // printf("teacher num: %d\n", student->teacher_num);
        RELEASE_STRUCT_OBJECT_ARRAY_START(teachers, teacher, teacher_num)
            RELEASE_STRUCT_MEMORY(name);
            RELEASE_STRUCT_PTR_ARRAY(course_names, course_num)
        RELEASE_STRUCT_OBJECT_ARRAY_END
        // printf("teacher: 0x%p\n", student->teacher);
        // printf("teacher num: %d\n", student->teacher_num);
        // printf("------ teacher ------\n");

        // printf("------ friends ------\n");
        // printf("friends: 0x%p\n", student->friends);
        // printf("friends num: %d\n", student->friends_num);
        RELEASE_STRUCT_PTR_ARRAY_START(friend, friends, friends_num)
            RELEASE_STRUCT_MEMORY(name);
            RELEASE_STRUCT_PTR_ARRAY_START(pet, pets, pet_num)
                RELEASE_STRUCT_MEMORY(pet_name);
            RELEASE_STRUCT_PTR_ARRAY_END
        RELEASE_STRUCT_PTR_ARRAY_END
        // printf("friends: 0x%p\n", student->friends);
        // printf("friends num: %d\n", student->friends_num);
        // printf("------ friends ------\n");

        // printf("zqq 0x%p\n", &student->friends_num);
        // printf("zqq 0x%p\n", &student->explain);
        // struct explain2* explain = &student->explain;
        // printf("zqq %s\n", explain->word_means);
        NESTED_STRUCT_START(explain2, explain)
            RELEASE_STRUCT_MEMORY(word_means);
            RELEASE_STRUCT_PTR_ARRAY(case_word, case_word_count);
        NESTED_STRUCT_END
    RELEASE_STRUCT_END;

    printf("allocated memory for student: 0x%p\n", student);

    return 0;
}
