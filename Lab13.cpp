/*
Файл student.dat
 фамилия, имя, отчество(40 символов);
 номер курса(1 символ);
 номер группы(1 символ);
 итоги сессий : 10 наборов из 5 оценок каждый;
 форма обучения : очная, очно - заочная(1 символ);
 экзамен : наименование предмета(10 символов) и оценка(1 символ);
Данные в файле упорядочены по номеру курса, внутри курса – по номеру группы, в группе – по фамилии.

Требуется занести в отдельный файл данные об отличниках из файла student.dat
*/

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

const int exam_name_len = 10;
const int full_name_len = 40;
const int count_set = 10;
const int count_marks = 5;

struct exam
{
    char exam_name[exam_name_len];
    int exam_mark;
};

struct student
{
private:
    char full_name[full_name_len];
    int course;
    int group;
    int session_marks[count_set][count_marks];
    char ed_form;
    exam result_exam;
public:
    //student(ifstream& file);
    student() = default;
    void input_from_txt(ifstream& file);
    void print();
    void print_to_txt(ofstream& file);
    bool condition();
};

//student::student(ifstream& file)
//{
//    file.getline(full_name, full_name_len);
//    file >> course;
//    file >> group;
//    for (int i = 0; i < count_set; i++)
//    {
//        for (int j = 0; j < count_marks; j++)
//        {
//            file >> session_marks[i][j];
//        }
//    }
//    file >> ed_form;
//    file.ignore();
//    file.getline(result_exam.exam_name, exam_name_len);
//    file >> result_exam.exam_mark;
//    if (!file.eof())
//    {
//        char separator[full_name_len];
//        file.ignore();
//        file.getline(separator, full_name_len);
//    }
//}

void student::input_from_txt(ifstream& file)
{
    file.getline(full_name, full_name_len);
    file >> course;
    file >> group;
    for (int i = 0; i < count_set; i++)
    {
        for (int j = 0; j < count_marks; j++)
        {
            file >> session_marks[i][j];
        }
    }
    file >> ed_form;
    file.ignore();
    file.getline(result_exam.exam_name, exam_name_len);
    file >> result_exam.exam_mark;
    if (!file.eof())
    {
        char separator[full_name_len];
        file.ignore();
        file.getline(separator, full_name_len);
    }
}

void student::print()
{
    cout << "ФИО: " << full_name << endl;
    cout << "Курс: " << course << endl;
    cout << "Группа: " << group << endl;
    cout << "Результаты сессий: " << endl;
    for (int i = 0; i < count_set; i++)
    {
        for (int j = 0; j < count_marks; j++)
        {
            cout << session_marks[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Форма обучения: ";
    switch (ed_form)
    {
    case 'о': cout << "Очная " << endl; break;
    case 'з': cout << "Очно-заочная " << endl; break;
    default: break;
    }
    cout << "Результат экзамена: ";
    cout << result_exam.exam_name << " - " << result_exam.exam_mark << endl;
}

void student::print_to_txt(ofstream& file)
{
    file << "ФИО: " << full_name << '\n';
    file << "Курс: " << course << '\n';
    file << "Группа: " << group << '\n';
    file << "Форма обучения: ";
    switch (ed_form)
    {
    case 'о': file << "Очная " << '\n'; break;
    case 'з': file << "Очно-заочная " << '\n'; break;
    }
}

bool student::condition()
{
    bool check = true;
    for (int i = 0; i < count_set; i++)
    {
        for (int j = 0; j < count_marks; j++)
        {
            if (session_marks[i][j] != 5)
            {
                check = false;
                goto label;
            }
        }
    }
    if (result_exam.exam_mark != 5)
    {
        check = false;
    }
label:
    return check;
}

void bin_output(student* students, int n)
{
    ofstream bin_file("result.bin", ios::binary);
    for (int i = 0; i < n; i++)
    {
        if ((students + i)->condition())
        {
            bin_file.write((char*)(students + i), sizeof(student));
            bin_file.flush();
        }
    }
    bin_file.close();
}

void txt_output(student* students, int n)
{
    ofstream txt_file("result.txt");
    for (int i = 0; i < n; i++)
    {
        if ((students + i)->condition())
        {
            (students + i)->print_to_txt(txt_file);
        }
    }
    txt_file.close();
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream file_input("student.dat");
    int n;
    file_input >> n;
    file_input.ignore();

    student* students = new student[n];
    for (int i = 0; i < n; i++)
    {
        //students[i] = student(file_input);
        students[i].input_from_txt(file_input);
    }
    file_input.close();

    cout << "Полученные данные из файла: " << endl;
    for (int i = 0; i < n; i++)
    {
        (students + i)->print();
        cout << endl;
    }

    int menu;
    cout << "В файл какого типа записать подходящих студентов? " << endl << "1.bin" << endl << "2.txt" << endl << "Ваш выбор: ";
    cin >> menu;
    switch (menu)
    {
    case 1:
        bin_output(students, n);
        cout << "Файл создан, данные записаны" << endl << endl;
        break;
    case 2:
        txt_output(students, n);
        cout << "Файл создан, данные записаны" << endl << endl;
        return 0;
    default:
        cout << "Неверный выбор" << endl;
        break;
    }

    delete[] students;

    ifstream bin_read("result.bin", ios::binary);
    student st;
    while (bin_read.peek() != EOF)
    {
        bin_read.read((char*)&st, sizeof(student));
        st.print();
    }
    bin_read.close();
}
