/**
 * @file TSingleton.hpp
 * @brief Definește template-ul generic pentru pattern-ul Singleton.
 */
#pragma once

namespace ls {

/**
 * @brief Template generic pentru pattern-ul de design Singleton.
 *
 * Asigură că o clasă are o singură instanță și oferă un punct global
 * de acces la aceasta. Clasa care moștenește @c Singleton trebuie să
 * aibă un constructor fără argumente accesibil acestui template
 * (de obicei prin declararea @c friend class Singleton<T>).
 *
 * Operațiile de copiere și mutare sunt șterse pentru a preveni
 * crearea de copii ale instanței unice.
 *
 * @tparam T Tipul clasei care implementează pattern-ul Singleton.
 */
template <class T> class Singleton {
public:
  /**
   * @brief Returnează referința la instanța unică a clasei.
   * @return Referință la instanța statică locală de tip @c T.
   */
  static T &getInstance() {
    static T instance;
    return instance;
  }

protected:
  /// @brief Constructor protejat implicit.
  Singleton() = default;

  /// @brief Destructor protejat implicit.
  ~Singleton() = default;

  /// @brief Constructorul de copiere este șters.
  Singleton(const Singleton &) = delete;

  /// @brief Operatorul de atribuire prin copiere este șters.
  Singleton &operator=(const Singleton &) = delete;

  /// @brief Constructorul de mutare este șters.
  Singleton(Singleton &&) = delete;

  /// @brief Operatorul de atribuire prin mutare este șters.
  Singleton &operator=(Singleton &&) = delete;
};

} // namespace ls
