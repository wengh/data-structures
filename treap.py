import collections
import random

Node = collections.namedtuple('Node', ['x', 'y', 'lc', 'rc'])

def node_new(x):
    return Node(x, random.uniform(0, 1), None, None)

def node_split(node, x):
    if not node:
        return None, None
    elif node.x <= x:
        left, right = node_split(node.rc, x)
        return node._replace(rc=left), right
    else:
        left, right = node_split(node.lc, x)
        return left, node._replace(lc=right)

def node_merge(left, right):
    if not left or not right:
        return left or right
    if left.y < right.y:
        return right._replace(lc=node_merge(left, right.lc))
    else:
        return left._replace(rc=node_merge(left.rc, right))

def node_str(node):
    if not node:
        return ''
    return f'({node_str(node.lc)} {node.x} {node_str(node.rc)})'

def node_iter(node):
    if node:
        yield from node_iter(node.lc)
        yield node.x
        yield from node_iter(node.rc)

def node_riter(node):
    if node:
        yield from node_iter(node.rc)
        yield node.x
        yield from node_iter(node.lc)

def node_add(node, x):
    left, right = node_split(node, x)
    return node_merge(left, node_merge(node_new(x), right))

def node_discard(node, x):
    left, right = node_split(node, x)
    left, _ = node_split(left, x-1)
    return node_merge(left, right)
