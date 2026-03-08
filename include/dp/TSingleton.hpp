#pragma once

namespace ls {

// Necesare ca sa mearga templateul:
// constructor fara args public/ friend class
template <class T> class Singleton {
public:
  static T &getInstance() {
    static T instance;
    return instance;
  }

protected:
  Singleton() = default;
  ~Singleton() = default;

  // Sterge copy/move
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  Singleton(Singleton &&) = delete;
  Singleton &operator=(Singleton &&) = delete;
};
} // namespace ls
