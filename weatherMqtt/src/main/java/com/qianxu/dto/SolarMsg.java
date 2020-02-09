package com.qianxu.dto;

public class SolarMsg{
    //时角
    private Double hour_angle;
    //太阳方位角
    private Double solar_azimuth_angle;
    //太阳高度角
    private Double solar_elevation_angle;
    //太阳时
    private Integer solar_hour;

    public Double getHour_angle() {
        return hour_angle;
    }

    public void setHour_angle(Double hour_angle) {
        this.hour_angle = hour_angle;
    }

    public Double getSolar_azimuth_angle() {
        return solar_azimuth_angle;
    }

    public void setSolar_azimuth_angle(Double solar_azimuth_angle) {
        this.solar_azimuth_angle = solar_azimuth_angle;
    }

    public Double getSolar_elevation_angle() {
        return solar_elevation_angle;
    }

    public void setSolar_elevation_angle(Double solar_elevation_angle) {
        this.solar_elevation_angle = solar_elevation_angle;
    }

    public Integer getSolar_hour() {
        return solar_hour;
    }

    public void setSolar_hour(Integer solar_hour) {
        this.solar_hour = solar_hour;
    }

    @Override
    public String toString() {
        return "SolarMsg{" +
                "hour_angle=" + hour_angle +
                ", solar_azimuth_angle=" + solar_azimuth_angle +
                ", solar_elevation_angle=" + solar_elevation_angle +
                ", solar_hour=" + solar_hour +
                '}';
    }
}