from ultralytics import YOLOWorld

# Initialize a YOLO-World model
model = YOLOWorld("yolov8s-world.pt")  # or select yolov8m/l-world.pt for different sizes

# model = YOLOWorld("custom_yolov8s.pt") 
# model.set_classes(["recyclable waste", "hazardous waste", "food wastes", "Other Waste"])
# model.save("custom_yolov8s.pt")

# Execute inference with the YOLOv8s-world model on the specified image
results = model.predict(r"D:\BaiduNetdiskDownload\product_class\product_class\upper_computer\data\pic\image.png")

result = results[0]
# 遍历所有检测框
for box in result.boxes:
    xyxy = box.xyxy[0].tolist()    # 边界框坐标 [x1,y1,x2,y2]
    conf = box.conf.item()          # 置信度
    cls_id = int(box.cls.item())    # 类别ID
    cls_name = result.names[cls_id] # 类别名称
    print(cls_name)

# # Show results
results[0].show()