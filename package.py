# -*- coding: utf-8 -*-

name = 'VDB_Deformer'

version = '0.0.1'

variants = [ ["gcc-4.8"] ]

requires = [ "houdini-16" ]

def commands():
    env.HOUDINI_DSO_PATH.append('{this.root}/dso')
