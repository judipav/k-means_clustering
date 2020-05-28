#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <conio.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

//using namespace std;

const int KK = 10;      //Количество кластеров
const int max_iterations = 100;     //Количетсво итераций

typedef struct {
    double x = 0;
    double y = 0;
    std::string name;
} apt;

class K_means {
private:
    std::vector<apt> apteka;
    int q_klaster;
    int k_apteka;
    std::vector<apt> centr;
    void identify_centers();

    inline double compute(apt k1, apt k2) {
        return sqrt(pow((k1.x - k2.x), 2) + pow((k1.y - k2.y), 2));
    }

    inline double compute_s(double a, double b) {
        return (a + b) / 2;
    }
public:
    K_means() : q_klaster(0), k_apteka(0) {};
    K_means(int n, apt* mas, int n_klaster);
    K_means(int n_klaster, std::istream& os);
    void clustering(std::ostream& os);
    void print()const;
    ~K_means();
    friend std::ostream& operator<<(std::ostream& os, const K_means& k);
};



std::ostream& operator<<(std::ostream& os, const K_means& k)
{
    os << "Начальные аптеки:" << std::endl;
    for (int i = 0; i < k.k_apteka; i++)
    {
        os << i << ".\t" << k.apteka[i].name << "\t\t" << k.apteka[i].x << " " << k.apteka[i].y << std::endl;
    }
    os << "Случайные начальные центры кластеризации: " << std::endl;
    for (int i = 0; i < k.q_klaster; i++)
    {
        os << i << ".\t" << k.centr[i].name << "\t\t" << k.centr[i].x << " " << k.centr[i].y << std::endl;
    }
    os << "\nКоличество кластеров:\t\t" << k.q_klaster << std::endl;
    os << "Количество аптек:\t\t" << k.k_apteka << std::endl;
    return os;
}

void K_means::identify_centers()
{
    srand((unsigned)time(NULL));
    apt temp;
    apt* mas = new apt[q_klaster];
    for (int i = 0; i < q_klaster; i++)
    {
        temp = apteka[0 + rand() % k_apteka];
        for (int j = 0; j < q_klaster; j++)
        {
            if (temp.x != mas[j].x && temp.y != mas[j].y)
            {
                mas[j] = temp;
            }
            else
            {
                i--;
                break;
            }
        }
    }
    for (int i = 0; i < q_klaster; i++)
    {
        centr.push_back(mas[i]);
    }
    delete[]mas;
}

K_means::K_means(int n, apt* mas, int n_klaster)
{
    for (int i = 0; i < n; i++)
    {
        apteka.push_back(*(mas + i));
    }
    q_klaster = n_klaster;
    k_apteka = n;
    identify_centers();
}

K_means::K_means(int n_klaster, std::istream& os) :q_klaster(n_klaster) 
{
    apt temp;
    while (os >> temp.x && os >> temp.y)
    {
        apteka.push_back(temp);
    }
    k_apteka = apteka.size();
    identify_centers();
}

void K_means::clustering(std::ostream& os)
{
    os << "\n\nНачало кластеризации:" << std::endl;
    std::vector<int> check_1(k_apteka, -1);
    std::vector<int> check_2(k_apteka, -2);
    int iter = 0;
    while (true)
    {
        os << "\n\n>>>\t\tИтерация №" << iter << "<<<\n\n";
        {
            for (int j = 0; j < k_apteka; j++)
            {
                double* mas = new double[q_klaster];
                for (int i = 0; i < q_klaster; i++)
                {
                    *(mas + 1) = compute(apteka[j], centr[i]);
                    os << "Расстояние от аптеки\t" << apteka[j].name << "\tк центру #" << i << ": \t" << *(mas + i) << std::endl;
                }
                double min_dist = *mas;
                int m_k = 0;
                for (int i = 0; i < q_klaster; i++)
                {
                    if (min_dist > * (mas + i))
                    {
                        min_dist = *(mas + i);
                        m_k = i;
                    }
                }
                os << "Минимальное расстояние к центру #" << m_k << std::endl;
                os << "Пересчитываем центр #" << m_k << ": ";
                centr[m_k].x = compute_s(apteka[j].x, centr[j].x);
                centr[m_k].y = compute_s(apteka[j].y, centr[j].y);
                os << centr[m_k].x << " " << centr[m_k].y << std::endl;
                delete[] mas;
            }

            int* mass = new int[k_apteka];
            os << "\nПроведем классификацию аптек: " << std::endl;
            for (int k = 0; k < k_apteka; k++) {
                double* mas = new double[q_klaster];
                for (int i = 0; i < q_klaster; i++)
                {
                    *(mas + i) = compute(apteka[k], centr[i]);
                    os << "Расстояние от аптеки №" << apteka[k].name << " к центру #" << i << ": " << *(mas + i) << std::endl;
                }
                double min_dist = *mas;
                int m_k = 0;
                for (int i = 0; i < q_klaster; i++)
                {
                    if (min_dist > * (mas + i))
                    {
                        min_dist = *(mas + i);
                        m_k = i;
                    }
                }
                mass[k] = m_k;
                os << "Аптека №" << k << " ближе всего к центру #" << m_k << std::endl;
            }
            os << "\nМассив соответствия аптек и центров: \n";
            for (int i = 0; i < k_apteka; i++)
            {
                os << mass[i] << " ";
                check_1[i] = *(mass + i);
            }
            os << std::endl << std::endl;

            os << "Результат кластеризации: " << std::endl;
            int itr = KK + 1;
            for (int i = 0; i < q_klaster; i++)
            {
                os << "Кластер #" << i << std::endl;
                for (int j = 0; j < k_apteka; j++)
                {
                    if (mass[j]==i)
                    {
                        os << apteka[j].name << "\t" << apteka[j].x << "\t" << apteka[j].y << std::endl;
                        mass[j] = ++itr;
                    }
                }
            }
            delete[] mass;
            os << "Новые центры: \n";
            for (int i = 0; i < q_klaster; i++)
            {
                os << centr[i].name << "\t" << centr[i].x << centr[i].y << std::endl;
            }
        }
        iter++;
        if (check_1 == check_2 || iter >= max_iterations) {
            break;
        }
        check_2 = check_1;
    }
    os << "\n\n\t\tЗавершение кластеризации." << std::endl;
}
K_means::~K_means()
{
}


int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    apt mas[15];
    mas[0].x = 53.089499;
    mas[0].y = 49.987911;
    mas[0].name = "124";
    mas[1].x = 53.092598;
    mas[1].y = 49.982308;
    mas[1].name = "118";
    mas[2].x = 53.095035;
    mas[2].y = 49.974463;
    mas[2].name = "174";
    mas[3].x = 53.091301;
    mas[3].y = 49.971501;
    mas[3].name = "173";
    mas[4].x = 53.095807;
    mas[4].y = 49.969633;
    mas[4].name = "325";
    mas[5].x = 53.09702;
    mas[5].y = 49.967575;
    mas[5].name = "763";
    mas[6].x = 53.087929;
    mas[6].y = 49.960574;
    mas[6].name = "695";
    mas[7].x = 53.096859;
    mas[7].y = 49.962924;
    mas[7].name = "359";
    mas[8].x = 53.099524;
    mas[8].y = 49.96122;
    mas[8].name = "9";
    mas[9].x = 53.101829;
    mas[9].y = 49.95824;
    mas[9].name = "399";
    mas[10].x = 53.087658;
    mas[10].y = 49.950998;
    mas[10].name = "64";
    mas[11].x = 53.10057;
    mas[11].y = 49.954606;
    mas[11].name = "29";
    mas[12].x = 53.103638;
    mas[12].y = 49.95331;
    mas[12].name = "69";
    mas[13].x = 53.096322;
    mas[13].y = 49.948836;
    mas[13].name = "357";
    mas[14].x = 53.100075;
    mas[14].y = 49.945604;
    mas[14].name = "474";
    K_means cluster = K_means(15, mas, 4);
    cluster.clustering(std::cout);
    std::getchar();
}