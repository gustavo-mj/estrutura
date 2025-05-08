// Copyright 2025 Gustavo Monteiro Jorge
#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>


namespace structures {

//! ...
template<typename T>
class LinkedList {
 public:
    //! ...
    LinkedList();  // construtor padrão
    //! ...
    ~LinkedList();  // destrutor
    //! ...
    void clear();  // limpar lista
    //! ...
    void push_back(const T& data);  // inserir no fim
    //! ...
    void push_front(const T& data);  // inserir no início
    //! ...
    void insert(const T& data, std::size_t index);  // inserir na posição
    //! ...
    void insert_sorted(const T& data);  // inserir em ordem
    //! ...
    T& at(std::size_t index);  // acessar um elemento na posição index
    //! ...
    T pop(std::size_t index);  // retirar da posição
    //! ...
    T pop_back();  // retirar do fim
    //! ...
    T pop_front();  // retirar do início
    //! ...
    void remove(const T& data);  // remover específico
    //! ...
    bool empty() const;  // lista vazia
    //! ...
    bool contains(const T& data) const;  // contém
    //! ...
    std::size_t find(const T& data) const;  // posição do dado
    //! ...
    std::size_t size() const;  // tamanho da lista

 private:
    class Node {  // Elemento (implementação pronta)
     public:
        explicit Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {  // getter: dado
            return data_;
        }

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }

        const Node* next() const {  // getter const: próximo
            return next_;
        }

        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_{nullptr};
    };

    Node* before_index(std::size_t index) {  // nó anterior ao 'index'
        auto it = head;
        for (auto i = 1u; i < index; ++i) {
            it = it->next();
        }
        return it;
    }

    Node* head{nullptr};
    Node* tail{nullptr};
    std::size_t size_{0u};
};

}  // namespace structures

#endif


//! Construtor
template<typename T>
structures::LinkedList<T>::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size_ = 0u;
}

//! Destrutor
template<typename T>
structures::LinkedList<T>::~LinkedList() {
    clear();
}

//! Esvazia
template<typename T>
void structures::LinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

//! Inserção no início
template<typename T>
void structures::LinkedList<T>::push_front(const T& data) {
    Node *novo;
    novo = new Node(data, head);
    if (novo == nullptr) {
        throw std::out_of_range("memória insuficiente");
    }
    if (head == nullptr) {
        tail = novo;
    }
    head = novo;
    size_++;
}

//! Inserção no fim
template<typename T>
void structures::LinkedList<T>::push_back(const T& data) {
    Node *novo;
    if (empty()) {
        return push_front(data);
    }
    novo = new Node(data, nullptr);
    if (novo == nullptr) {
        throw std::out_of_range("memória insuficiente");
    }
    tail->next(novo);
    tail = novo;
    size_++;
}

//! Dado da posição 'index'
template<typename T>
T& structures::LinkedList<T>::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("índice fora dos limites");
    }
    Node* current = head;
    for (std::size_t i = 0; i < index; i++) {
        current = current->next();
    }
    return current->data();
}

//! Inserção na posição 'index'
template<typename T>
void structures::LinkedList<T>::insert(const T& data, std::size_t index) {
    if (index > size_) {
        throw std::out_of_range("índice fora dos limites");
    }
    if (index == 0) {
        push_front(data);
    } else if (index == size_) {
        push_back(data);
    } else {
        Node* prev = before_index(index);
        Node* new_node = new Node(data, prev->next());
        if (new_node == nullptr) {
            throw std::out_of_range("memória insuficiente");
        }
        prev->next(new_node);
        ++size_;
    }
}

//! Inserção ordenada
template<typename T>
void structures::LinkedList<T>::insert_sorted(const T& data) {
    if (empty() || data < head->data()) {
        push_front(data);
    } else {
        Node* prev = head;
        std::size_t index = 1;
        while (prev->next() != nullptr && data > prev->next()->data()) {
            prev = prev->next();
            ++index;
        }
        insert(data, index);
    }
}

//! Remoção do início
template<typename T>
T structures::LinkedList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    T aux;
    Node *p;
    p = head;
    head = p->next();
    aux = p->data();
    delete p;
    if (head == nullptr) {
        tail = nullptr;
    }
    size_--;
    return aux;
}

//! Remoção do fim
template<typename T>
T structures::LinkedList<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    if (size() == 1) {
        return pop_front();
    }
    T aux;
    Node *p, *ant;
    ant = nullptr;
    p = head;
    while (p != tail) {  // 'ant' deve apontar para o penúltimo nó
        ant = p;
        p = p->next();
    }
    aux = p->data();
    delete p;
    if (ant == nullptr) {
        head = nullptr;
    } else {
        ant->next(nullptr);
    }
    tail = ant;
    size_--;
    return aux;
}

//! Remoção da posição 'index'
template<typename T>
T structures::LinkedList<T>::pop(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("índice fora dos limites");
    }
    if (index == 0) {
        return pop_front();
    } else if (index == size_ - 1) {
        return pop_back();
    } else {
        Node* prev = before_index(index);
        Node* to_remove = prev->next();
        T data = to_remove->data();
        prev->next(to_remove->next());
        delete to_remove;
        --size_;
        return data;
    }
}

//! Remoção de um dado
template<typename T>
void structures::LinkedList<T>::remove(const T& data) {
    pop(find(data));
}

//! Verificação de vazia
template<typename T>
bool structures::LinkedList<T>::empty() const {
    return size() == 0u;
}

//! Verificação se contém um dado
template<typename T>
bool structures::LinkedList<T>::contains(const T& data) const {
    return find(data) != size();
}

//! Índice de um dado (se existir); ou 'size() (se não existir)
template<typename T>
std::size_t structures::LinkedList<T>::find(const T& data) const {
    Node* current = head;
    std::size_t index = 0;
    while (current != nullptr) {
        if (current->data() == data) {
            return index;
        }
        current = current->next();
        ++index;
    }
    return size_;
}

//! Quantidade atual de elementos
template<typename T>
std::size_t structures::LinkedList<T>::size() const {
    return size_;
}

// QUESTÕES DA PROVA PRÁTICA


// método inversor
template<typename T>
void structures::LinkedList<T>::reverse() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    } else if (size_ == 1) {
        return;
    }
    std::size_t size_original = size();
    for (std::size_t i = 1; i < size_original; ++i) {
        push_front(pop_back());
    }
}

// método de duplicação
template<typename T>
void structures::LinkedList<T>::duplicate() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    std::size_t size_original = size();
    for (std::size_t i = 0; i < size_original; ++i) {
        push_back(at(i));
    }
}

// método slicing
template<typename T>
structures::LinkedList<T> structures::LinkedList<T>::slicing(int start, int stop, int step) {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }
    if (start < 0 || stop >= static_cast<int>(size()) || step <= 0) {
        throw std::invalid_argument("parâmetros inválidos");
    }

    structures::LinkedList<T> result;

    for (int i = start; i <= stop; i += step) {
        result.insert_sorted(at(i));
    }
    return result;
}

// método troca meios
template<typename T>
void structures::LinkedList<T>::exchange(structures::LinkedList<T>* lista_in) {
    if (empty() || lista_in->empty()) {
        throw std::out_of_range("lista vazia");
    }
    std::size_t passo = size_ / 2;
    structures::LinkedList<T> temporario1; // lista1 = [A,B,C,D,E,F]
    structures::LinkedList<T> temporario2; // lista2 = [1,2,3,4,5,6]

    for (std::size_t i = 0; i < passo; ++i) {
        temporario1.push_back(pop_back());             // temporario1 = [F,E,D] e L1 = [A,B,C]
        temporario2.push_back(lista_in->pop_back());   // temporario2 = [6,5,4] e L2 = [1,2,3]
    }
    for (std::size_t i = 0; i < passo; ++i) {
        push_back(temporario2.pop_back());             // L1 = [A,B,C,4,5,6]
        lista_in->push_back(temporario1.pop_back());   // L2 = [1,2,3,D,E,F]
    }
}

// método decompor por paridade
template<typename T>
structures::LinkedList<structures::LinkedList<T>*> structures::LinkedList<T>::decompose() {
    if (empty()) {
        throw std::out_of_range("lista vazia");
    }

    auto* pares = new structures::LinkedList<T>();
    auto* impares = new structures::LinkedList<T>();

    for (std::size_t i = 0; i < size(); ++i) {
        if (i % 2 == 0) {
            pares->push_back(at(i));
        } else {
            impares->push_back(at(i));
        }
    }

    structures::LinkedList<structures::LinkedList<T>*> result;
    result.push_back(pares);
    result.push_back(impares);

    return result;
}