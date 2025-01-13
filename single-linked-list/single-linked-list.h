#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node){}

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_){}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(this->node_ != nullptr);
            this->node_ = this->node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(this->node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(this->node_ != nullptr);
            return &node_->value ;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() {};

    SingleLinkedList(std::initializer_list<Type> values) {
        Assign(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Assign(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        auto rhs_copy(rhs);
        this->swap(rhs_copy);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_node , other.head_.next_node);
        std::swap(this->size_, other.size_);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while(!this->IsEmpty()){
            auto tmp_ptr = head_.next_node -> next_node;
            delete head_.next_node;
            head_.next_node = tmp_ptr;
            --size_;
        }
    }

    ~SingleLinkedList(){
        Clear();
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node *>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        auto tmp_ptr = pos.node_->next_node;
        pos.node_->next_node = new Node(value, tmp_ptr);
        pos.node_->next_node->next_node = tmp_ptr;
        ++size_;

        return Iterator{pos.node_->next_node};
    }

    void PopFront() noexcept {
        if(!this->IsEmpty()){
            auto tmp_ptr = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = tmp_ptr;
            --size_;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if(!this->IsEmpty() && pos.node_ != nullptr && pos.node_->next_node != nullptr){
            auto tmp_ptr = pos.node_->next_node->next_node;
            delete pos.node_->next_node;
            pos.node_->next_node = tmp_ptr;
            --size_;
            return Iterator{pos.node_->next_node};
        }
        return this->end();
    }

private:
    Node head_ = Node();
    size_t size_ = 0;

    template <typename It>
    void Assign(It start, It end){
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList<Type> tmp;
        Node* last_node = &tmp.head_;
        while(start != end) {
            auto tmp_node = new Node(*start, nullptr);
            last_node->next_node = tmp_node;
            last_node = tmp_node;
            ++tmp.size_;
            ++start;
        }
        swap(tmp);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(&lhs == &rhs){
        return true;
    }
    if(lhs.GetSize() != rhs.GetSize()){
        return false;
    }
    bool equal = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    return equal;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

