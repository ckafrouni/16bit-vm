pub mod errors;

pub use errors::*;

use super::Device;

// Device is a trait that represents a device that can be mapped to the memory
// For example, our byte_array::ByteArray implements this trait

#[allow(dead_code)]
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum AccessFlags {
    ReadExecute,
    ReadWrite,
    ReadWriteExecute,
    ReadOnly,
    WriteOnly,
}

pub struct Region {
    device: Box<dyn Device>,
    start: u32,
    end: u32,
    flags: AccessFlags,
    // remap: Option<(u32, u32)>,
}

impl std::fmt::Debug for Region {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Region")
            .field("start", &format_args!("{:#010x}", self.start))
            .field("end", &format_args!("{:#010x}", self.end))
            .field("flags", &self.flags)
            .finish()
    }
}

#[derive(Debug)]
pub struct MMU {
    regions: Vec<Region>,
}

impl MMU {
    pub fn new() -> Self {
        Self {
            regions: Vec::new(),
        }
    }

    pub fn map(
        &mut self,
        device: Box<dyn Device>,
        addr: u32,
        size: u32,
        flags: AccessFlags,
    ) -> Result<impl FnOnce(), MMUError> {
        let region = Region {
            device,
            start: addr,
            end: addr + size,
            flags,
            // remap: None,
        };

        self.regions.insert(0, region);
        Ok(|| todo!("Unmapping regions is not implemented yet"))
    }

    fn find_region(&self, addr: u32) -> Option<&Region> {
        self.regions
            .iter()
            .find(|region| region.start <= addr && region.end > addr)
    }

    fn find_region_mut(&mut self, addr: u32) -> Option<&mut Region> {
        self.regions
            .iter_mut()
            .find(|region| region.start <= addr && region.end > addr)
    }

    pub fn read8(&self, addr: u32) -> Result<u8, MMUError> {
        let region = self.find_region(addr).ok_or(MMUError::InvalidAddress)?;
        if region.flags == AccessFlags::WriteOnly {
            return Err(MMUError::IllegalMemoryAccess);
        }

        let offset = addr - region.start;
        region.device.read8(offset).map_err(|err| err.into())
    }

    pub fn read32(&self, addr: u32) -> Result<u32, MMUError> {
        let region = self.find_region(addr).ok_or(MMUError::InvalidAddress)?;
        if region.flags == AccessFlags::WriteOnly {
            return Err(MMUError::IllegalMemoryAccess);
        }

        let offset = addr - region.start;
        region.device.read32(offset).map_err(|err| err.into())
    }

    #[allow(dead_code)]
    pub fn write8(&mut self, addr: u32, value: u8) -> Result<(), MMUError> {
        let region = self.find_region_mut(addr).ok_or(MMUError::InvalidAddress)?;
        if region.flags == AccessFlags::ReadOnly {
            return Err(MMUError::IllegalMemoryAccess);
        }

        let offset = addr - region.start;
        region
            .device
            .write8(offset, value)
            .map_err(|err| err.into())
    }

    pub fn write32(&mut self, addr: u32, value: u32) -> Result<(), MMUError> {
        let region = self.find_region_mut(addr).ok_or(MMUError::InvalidAddress)?;
        if region.flags == AccessFlags::ReadOnly {
            return Err(MMUError::IllegalMemoryAccess);
        }

        let offset = addr - region.start;
        region
            .device
            .write32(offset, value)
            .map_err(|err| err.into())
    }
}
