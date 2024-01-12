#![allow(dead_code)]
#[derive(Debug)]
pub enum DeviceError {
    InvalidAddress,
    IllegalMemoryAccess,
    DeviceError,
}

pub trait Device
where
    Self: std::fmt::Debug,
{
    fn read(&self, addr: u32, size: u32) -> Result<Vec<u8>, DeviceError>;
    fn read8(&self, addr: u32) -> Result<u8, DeviceError>;
    fn read32(&self, addr: u32) -> Result<u32, DeviceError>;

    fn write(&mut self, addr: u32, size: u32, value: u32) -> Result<(), DeviceError>;
    fn write8(&mut self, addr: u32, value: u8) -> Result<(), DeviceError>;
    fn write32(&mut self, addr: u32, value: u32) -> Result<(), DeviceError>;

    fn slice(&self, start: u32, end: u32) -> Result<Vec<u8>, DeviceError> {
        self.read(start, end - start)
    }
}

// TODO: implement this
// pub trait SliceableDevice: Box<dyn Device>evice {
//     fn slice(&self, start: u32, end: u32) -> Self;
// }
