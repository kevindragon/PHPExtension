#!/usr/bin/env python
# encoding=utf-8

import threading, time, re
import config
from sphinxapi import *

class SearchManager(threading.Thread):
    """
    initialize parameters and create a thread
    """
    def __init__(self, no = 0, query_data = [], host = None, port = None):
        threadname = '%s' % no
        threading.Thread.__init__(self, name = threadname)    # call parent __init__ function to create a new thread
        self.no = no                                          # varibale no used to record which index to record
        self.query_data = query_data                          # put search result from sphinx into this variable
        self.query_data[self.no] = None
        self.text_term = []
        
        if host is not None:
            self.host = host
        else:
            self.host = config.sphinx['host']
        if port is not None:
            self.port = port
        else:
            self.port = config.sphinx['port']

        self.sphinx = SphinxClient()                          # we need a sphinx instance
        self.sphinx.SetServer(self.host, self.port)
        self.sphinx.SetMatchMode(SPH_MATCH_EXTENDED2)
        self.sphinx.SetFieldWeights(config.weight)

    def runQuery(self):
        """
        run sphinx query with commands early set
        """
        self.sphinx.SetLimits(self.params['limit'][0], self.params['limit'][1])
        return self.sphinx.RunQueries()

    def run(self):
        """
        start this thread
        """
        res = self.runQuery()
        self.query_data[self.no] = res

    def setParams(self, params):
        """
        set all parameters to sphinx object
        """
        self.params = params
        
        self.sphinx.SetSortMode(SPH_SORT_EXTENDED, self.params['order_by'])
        self.sphinx.SetLimits(self.params['limit'][0], self.params['limit'][1])
        # set all parameters
        self.order_by()
        self.effective()
        self.language()
        self.source()
        self.power_level()
        self.province()
        self.city()
        self.category()
        self.effect_date()
        self.issue_date()
        self.article()
        self.title()
        self.content()
        
        if len(self.text_term):
            self.sphinx.AddQuery(''.join(self.text_term), 'tax')
        else:
            self.sphinx.AddQuery('', 'tax')

    def order_by(self):
        """
        what order by
        """
        if 'order_by' in self.params:
            self.order_by = self.params['order_by']
        else :
            self.order_by = "issue_date DESC,@weight DESC"
            
    def effective(self):
        """
        effective of law
        """
        if 'effect_id' in self.params:
            self.sphinx.SetFilter('effect_id', self.params['effect_id'])
    
    def language(self):
        """
        set search language
        """
        if 'is_english' in self.params:
            self.sphinx.SetFilter('is_english', self.params['is_english'])
            
    def source(self):
        """
        source department
        """
        if 'source' in self.params:
            self.text_term.append('@source %s' % self.params['source'].encode('utf-8'));

    def power_level(self):
        """
        jurisdicational level
        """
        if 'power_level' in self.params:
            if type(self.params) is not list:
                self.params['power_level'] = [self.params['power_level']]
            self.sphinx.SetFilter('power_level', self.params['power_level'])
    
    def province(self):
        """
        set province
        """
        if 'province' in self.params:
            self.sphinx.SetFilter('province', self.params['province'])

    def city(self):
        """
        set city
        """
        if 'city' in self.params:
            self.sphinx.SetFilter('city', self.params['city'])

    def category(self):
        """
        set category
        """
        if 'taxonomy_id_list' in self.params:
            taxonomy_id_list = [int(x) for x in self.params['taxonomy_id_list']]
            self.sphinx.SetFilter('taxonomy_id_list', taxonomy_id_list)
    
    def effect_date(self):
        """
        set effect date into sphinx search conditions
        """
        if 'effect_date_ts' in self.params:
            self.sphinx.SetFilter('effect_date_ts', self.params['effect_date_ts'])

    def issue_date(self):
        """
        set issue date 
        """
        if 'issue_date' in self.params:
            self.sphinx.SetFilterRange('issue_date', self.params['issue_date'][0], self.params['issue_date'][1])

    def article(self):
        """
        set article which from input box
        """
        article = []
        if 'article_id' in self.params:
            if self.params['article_id'][0:1] != '"':
                article.append('"%s"' % self.params['article_id'])
            else :
                article.append(self.params['article_id'])

        if 'article_year' in self.params:
            article.append(self.params['article_year'])

        if 'article_num' in self.params:
            article.append(self.params['article_num'])

        if len(article):
            article_id = ('%s' % ('" "'.join(article))).replace('""', '"').strip('"');
            self.text_term.append('@article_id %s' % article_id.encode('utf-8'));
    
    def title(self):
        """
        title 
        """
        if 'title' in self.params:
            titles = self.params['title']
            self.text_term.append('@title %s' % titles.encode('utf-8'))
    
    def content(self):
        """
        content
        """
        if 'content' in self.params:
            contents = self.params['content']
            self.text_term.append('@content %s' % contents.encode('utf-8'));

    
