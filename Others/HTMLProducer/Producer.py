import sys
import os
class html_maker:
	
	def __init__(self, year, type_, cats, outputpath, outputname):
		self.year = year
		self.type_ = type_
		self.cats = cats
		self.head = '
