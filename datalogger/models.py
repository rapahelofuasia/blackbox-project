from django.db import models

class CarData(models.Model):
    longitude = models.FloatField()
    latitude = models.FloatField()
    alcohol = models.IntegerField()
    vibration = models.IntegerField()
    x_acceleration = models.IntegerField()
    y_acceleration = models.IntegerField()
    z_acceleration = models.IntegerField()
    created_at = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"Car Data #{self.pk}"
