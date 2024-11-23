# free-memory
C language method for freeing memory.🥛

可以方便 C 语言程序员释放内存的函数。

## 如何使用

```c
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
} Student;
```

上面这个结构体的释放可以简化为下面这样：

```c
int main()
{
    Student* student = create_student();

    RELEASE_STRUCT_START(Student, student)
        RELEASE_STRUCT_MEMORY(name)
        RELEASE_STRUCT_PTR_ARRAY(dog_names, dog_num);
        RELEASE_STRUCT_OBJECT_ARRAY_START(teachers, teacher, teacher_num)
            RELEASE_STRUCT_MEMORY(name);
            RELEASE_STRUCT_PTR_ARRAY(course_names, course_num)
        RELEASE_STRUCT_OBJECT_ARRAY_END
        RELEASE_STRUCT_PTR_ARRAY_START(friend, friends, friends_num)
            RELEASE_STRUCT_MEMORY(name);
            RELEASE_STRUCT_PTR_ARRAY_START(pet, pets, pet_num)
                RELEASE_STRUCT_MEMORY(pet_name);
            RELEASE_STRUCT_PTR_ARRAY_END
        RELEASE_STRUCT_PTR_ARRAY_END
    RELEASE_STRUCT_END;

    return 0;
}
```
