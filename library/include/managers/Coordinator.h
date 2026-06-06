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

    /** @brief Zwraca referencję do menedżera instruktorów. */
    InstructorManager& getInstructorManager();
    /** @brief Zwraca referencję do menedżera kursantów. */
    StudentManager& getStudentManager();
    /** @brief Zwraca referencję do menedżera sesji jazd. */
    DrivingSessionManager& getDrivingSessionManager();
    /** @brief Zwraca referencję do menedżera pojazdów. */
    VehicleManager& getVehicleManager();
    /** @brief Zwraca referencję do menedżera kursów. */
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

    /**
     * @brief Pobiera kursanta na podstawie jego unikalnego ID.
     * @param id Unikalny identyfikator kursanta.
     * @return Wskaźnik na obiekt kursanta (StudentPtr) lub nullptr, jeśli nie znaleziono.
     */
    StudentPtr getStudentById(int id);

    /**
     * @brief Pobiera instruktora na podstawie jego unikalnego ID.
     * @param id Unikalny identyfikator instruktora.
     * @return Wskaźnik na obiekt instruktora (InstructorPtr) lub nullptr, jeśli nie znaleziono.
     */
    InstructorPtr getInstructorById(int id);

    /**
     * @brief Pobiera pojazd na podstawie jego unikalnego ID.
     * @param id Unikalny identyfikator pojazdu.
     * @return Wskaźnik na obiekt pojazdu (VehiclePtr) lub nullptr, jeśli nie znaleziono.
     */
    VehiclePtr getVehicleById(int id);

    /**
     * @brief Pobiera kurs na podstawie jego unikalnego ID.
     * @param id Unikalny identyfikator kursu.
     * @return Wskaźnik na obiekt kursu (CoursePtr) lub nullptr, jeśli nie znaleziono.
     */
    CoursePtr getCourseById(int id);

    /**
     * @brief Pobiera sesję jazdy na podstawie jej unikalnego ID.
     * @param id Unikalny identyfikator sesji jazdy.
     * @return Wskaźnik na obiekt sesji (DrivingSessionPtr) lub nullptr, jeśli nie znaleziono.
     */
    DrivingSessionPtr getDrivingSessionById(int id);

    /**
     * @brief Znajduje ID aktywnego kursu dla danego kursanta.
     * @param studentID ID kursanta.
     * @return ID kursu lub -1 jeśli nie znaleziono.
     */
    int getCourseIDByStudentID(int studentID);
};
