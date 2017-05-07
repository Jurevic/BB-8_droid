# -*- coding: utf-8 -*-
from __future__ import absolute_import, unicode_literals

import requests
import json

from django.views.generic import TemplateView

from django.contrib.auth.mixins import LoginRequiredMixin
from django.shortcuts import render
from django.conf import settings
from django.http import HttpResponse

MOVE = 'https://api.particle.io/v1/devices/%s/move/' % settings.CLIENT_ID


class ControlView(LoginRequiredMixin, TemplateView):
    """
    Attaches headers when logging in
    in: email, password
    """

    def move(self, angle, speed, head_rotation=0):
        data = {
            'access_token': settings.CLIENT_SECRET,
            'arg': str(angle) + ' ' + str(speed) + ' ' + str(head_rotation)
        }
        response = requests.post(MOVE, data=data)
        return response.json()

    def get(self, request, *args, **kwargs):
        """
        Loads bb8 video stream with controls
        :param request: 
        :param args: 
        :param kwargs: 
        :return: 
        """
        

        return render(request, 'control/index.html')

    def post(self, request, *args, **kwargs):
        """
        POSTS to BB8 where to move
        :param request: 
        :param args: 
        :param kwargs: 
        :return: 
        """
        directions = {
            'left': 270,
            'right': 90,
            'up': 0,
            'down': 180
        }
        direction = directions.get(request.POST.get('direction'), 0)
        try:
            speed = int(request.POST.get('speed'))
            if not 0 <= int(speed) <= 255:
                return HttpResponse(json.dumps({'failed': 'Not in range! 0 - 255'}), content_type="application/json")
        except TypeError:
            return HttpResponse(json.dumps({'failed': 'Speed not an integer!'}), content_type="application/json")

        self.move(angle=direction, speed=speed)
        return HttpResponse(
            json.dumps({'success': 'direction: '+str(direction)+' speed: '+str(speed)}),
            content_type="application/json"
        )
