#pragma once

#include "typedefs.h"
#include <vector>

enum class Category;
class InstructorManager;
class StudentManager;
class DrivingSessionManager;
class VehicleManager;
class CourseManager;

    /**
     * @class Coordinator
     * @brief Główny koordynator systemu integrujący działanie wszystkich menedżerów.
     * * Klasa odpowiada za operacje biznesowe, takie jak obliczanie kosztów,
     * weryfikacja dostępności zasobów (pojazdów, instruktorów) oraz generowanie statystyk.
     */

class Coordinator {
private:
    InstructorManager& instructorManager;
    StudentManager& studentManager;
    DrivingSessionManager& drivingSessionManager;
    VehicleManager& vehicleManager;
    CourseManager& courseManager;



public:

    /**
      * @brief Konstruktor koordynatora.
      * @param instructorManager Referencja do menedżera instruktorów.
      * @param studentManager Referencja do menedżera kursantów.
      * @param drivingSessionManager Referencja do menedżera sesji jazd.
      * @param vehicleManager Referencja do menedżera pojazdów.
      * @param courseManager Referencja do menedżera kursów.
   */

    Coordinator(InstructorManager& instructorManger, StudentManager& studentManager, DrivingSessionManager& drivingSessionManager,
                VehicleManager& vehicleManger, CourseManager& courseManager);

    InstructorManager& getInstructorManager();
    StudentManager& getStudentManager();
    DrivingSessionManager& getDrivingSessionManager();
    VehicleManager& getVehicleManager();
    CourseManager& getCourseManager();

    /**
     * @brief Oblicza całkowity przychód ośrodka ze wszystkich kursów.
     * @return Całkowity przychód (double).
     */
    double calculateRevenue();

    /**
         * @brief Oblicza całkowity koszt konkretnego kursu.
         * @param courseId ID kursu.
         * @return Koszt kursu (double).
         */
    double calculateTotalCourseCost(int courseId);

    /**
         * @brief Oznacza kurs jako zakończony.
         * @param courseId ID kursu.
         */
    void completeCourse(int courseId);

    /**
     * @brief Sprawdza bilans kosztów kursanta.
     * @param studentId ID kursanta.
     * @return Zsumowany koszt kursów (double).
     */
    double checkStudentCourseBalance(int studentId);

    /**
         * @brief Zwraca całkowitą liczbę wyjeżdżonych godzin dla danego kursu.
         * @param courseId ID kursu.
         * @return Liczba godzin (double).
         */
    double getTotalHoursForCourse(int courseId);
    /**
     * @brief Zwraca całkowitą liczbę wyjeżdżonych godzin dla danego kursanta.
     * @param studentId ID kursanta.
     * @return Liczba godzin (double).
     */
    double getStudentHours(int studentId);

    /**
     * @brief Wyszukuje pojazdy dostępne w zadanym przedziale czasowym.
     * @param category Kategoria pojazdu.
     * @param range Przedział czasowy (TimeRange).
     * @return Wektor dostępnych pojazdów (VehiclePtr).
     */
    std::vector<VehiclePtr> getAvailableVehicles(Category category, TimeRange range);

    /**
     * @brief Wyszukuje instruktorów dostępnych w zadanym przedziale czasowym.
     * @param category Kategoria uprawnień.
     * @param range Przedział czasowy (TimeRange).
     * @return Wektor dostępnych instruktorów (InstructorPtr).
     */
    std::vector<InstructorPtr> getAvailableInstructors(Category category, TimeRange range);

    StudentPtr getStudentById(int id);
    InstructorPtr getInstructorById(int id);
    VehiclePtr getVehicleById(int id);
    CoursePtr getCourseById(int id);
    DrivingSessionPtr getDrivingSessionById(int id);

    /**
     * @brief Znajduje ID aktywnego kursu dla danego kursanta.
     * @param studentID ID kursanta.
     * @return ID kursu lub -1 jeśli nie znaleziono.
     */
    int getCourseIDByStudentID(int studentID);
};
