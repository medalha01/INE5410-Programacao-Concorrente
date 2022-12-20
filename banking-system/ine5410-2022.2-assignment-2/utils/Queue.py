

class Queue:

    class Node:
        def __init__(self, val: int):
            self.val: int = val
            self.next: self.Node = None
            self.prev: self.Node = None

    def __init__(self):
        head: self.Node = None
        tail: self.Node = None
    
    def push(self, val) -> None:
        new_node = self.Node(val)
        if self.is_empty():
            self.head = new_node
            self.tail = self.head
        else:
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
            

    def pop(self) -> int:
        if not self.is_empty():
            old_node = self.tail
            self.tail  = self.tail.prev
            self.tail.next = None
            return old_node.val


    def is_empty(self) -> bool:
        return self.head is None
    
