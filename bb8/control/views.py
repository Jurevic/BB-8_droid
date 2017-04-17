# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

from django.views.generic import TemplateView

from django.contrib.auth.mixins import LoginRequiredMixin


class ControlView(LoginRequiredMixin, TemplateView):
    pass
